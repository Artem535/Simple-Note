#include "storage.h"

void Storage::readNotesInformation(const QString &pathToFile) {
  if (!QFile::exists(pathToFile)) {
    auto functionName{"readNotesInformation"};
    QString message{"Error in function `%1`. File `%2` not exists."};
    message = message.arg(functionName).arg(pathToFile);
    std::runtime_error(message.toStdString());
  }

  QFile file(pathToFile);
  file.open(QIODevice::ReadOnly);
  noteInform = QJsonDocument::fromJson(file.readAll()).object();
  file.close();
}

void Storage::saveNotesInformation() {
  if (!QFile::exists(paths.inform)) {
    auto functionName{"saveNotesInformation"};
    QString message{"Error in function `%1`. File `%2` not exists."};
    message = message.arg(functionName).arg(paths.inform);
    std::runtime_error(message.toStdString());
  }

  QFile file(paths.inform);
  file.open(QIODevice::WriteOnly);
  QJsonDocument outputDoc;
  outputDoc.setObject(noteInform);
  file.write(outputDoc.toJson());
  file.close();
}

QString Storage::getStorageDirectory() {
  auto directory{QDir::home()};
  directory.mkdir(nameStorageDir);
  directory.cd(nameStorageDir);
  return directory.path();
}

void Storage::createFirstInitFiles() {
  // Create notes informations.
  QFile file{paths.inform};
  file.open(QIODevice::WriteOnly);
  QString emptyTemplate{"{\"%1\": [\"0\"], \"%2\": {}}"};
  emptyTemplate = emptyTemplate.arg(jsConst.freeId).arg(jsConst.notes);
  file.write(emptyTemplate.toStdString().c_str());
  file.close();

  // Create directory for text notes.
  QDir directory{paths.workDir};
  directory.mkdir(jsConst.notes);
}

void Storage::addNoteInInformFile(const QString &id, const QString &title) {
  // Create `value`: {"path to file": ..., "title": ...}
  QJsonObject value;
  value.insert(jsConst.pathToFile, getPathToText(id));
  value.insert(jsConst.noteTitle, title);
  // Qt can't change value 'complex' object.
  // We needed get object, insert new object in object, then replace `notes`
  // object.
  auto notesObject{noteInform[jsConst.notes].toObject()};
  notesObject.insert(id, value);
  noteInform[jsConst.notes] = notesObject;
}

QString Storage::getPathToText(const QString &id) {
  return paths.notesDir + id;
}

void Storage::saveNoteTextFile(const QString &id, const QString &text) {
  // If file not found, it will be create.
  QFile file{getPathToText(id)};
  file.open(QIODevice::WriteOnly);
  file.write(text.toStdString().c_str());
  file.close();
}

Storage::Storage() {
  nameStorageDir = ".SimpleNote";
  paths.workDir = getStorageDirectory() + "/";
  paths.notesDir = paths.workDir + "notes/";
  paths.inform = paths.workDir + "inform.json";

  if (!QFile::exists(paths.inform)) {
    createFirstInitFiles();
  }

  readNotesInformation(paths.inform);
}

QString Storage::getFreeId() {
  // From `value` we get QJsonValue, convert it to array.
  // Get first element array and convert it to int.
  auto array{noteInform.value(jsConst.freeId).toArray()};
  QString neededId;
  if (array.empty()) {
    neededId = QString::number(noteInform[jsConst.notes].toObject().size());
  } else {
    neededId = array.first().toString();
    array.pop_front();
    // Then replace `free id` array.
    noteInform[jsConst.freeId] = array;
  }
  return neededId;
}

void Storage::removeNote(QListWidgetItem *notePtr) {
  auto findedNote{noteIds.find(notePtr)};
  bool noteExist = findedNote != noteIds.end();
  if (noteExist) {
    auto id{findedNote->second};
    // Check that file exists.
    if (!QFile::exists(getPathToText(id))) {
      auto functionName{"removeNote"};
      QString message{"Error in function `%1`. File `%2` not exists."};
      message = message.arg(functionName).arg(getPathToText(id));
      std::runtime_error(message.toStdString());
    }

    // Remove text note
    QFile::remove(getPathToText(id));
    // Remove information about note.
    auto notesObject{noteInform[jsConst.notes].toObject()};
    notesObject.remove(id);
    noteInform[jsConst.notes] = notesObject;
    // Add `id` in array with free id.
    auto freeIdArray{noteInform[jsConst.freeId].toArray()};
    freeIdArray.push_back(id);
  }
  saveNotesInformation();
}

void Storage::addNote(QListWidgetItem *notePtr, const QString &title,
                      const QString &text) {
  bool itNewNote = noteIds.find(notePtr) == noteIds.end();
  if (itNewNote) {
    auto id{getFreeId()};
    saveNoteTextFile(id, text);
    addNoteInInformFile(id, title);
    noteIds.insert({notePtr, id});
  } else {
    renameNote(notePtr, title);
    changeNoteText(notePtr, text);
  }
  saveNotesInformation();
}

void Storage::renameNote(QListWidgetItem *notePtr, const QString &newTitle) {
  auto id{noteIds[notePtr]};
  auto notesObject{noteInform[jsConst.notes].toObject()};
  auto idObject{notesObject[id].toObject()};

  idObject[jsConst.noteTitle] = newTitle;
  notesObject[id] = idObject;
  noteInform[jsConst.notes] = notesObject;
}

void Storage::changeNoteText(QListWidgetItem *notePtr, const QString &newText) {
  auto findedNote{noteIds.find(notePtr)};
  if (findedNote != noteIds.end()) {
    QFile file(getPathToText(findedNote->second));
    file.open(QIODevice::WriteOnly);
    file.write(newText.toStdString().c_str());
    file.close();
  }
}

QString Storage::getTextFromNote(QListWidgetItem *notePtr) {
  QString result;
  // Find id note.
  auto findNoteIdPtr{noteIds.find(notePtr)};
  // If we find note, read text from file and return it,
  // else return empty string.
  if (findNoteIdPtr != noteIds.end()) {
    auto pathToFile{getPathToText(findNoteIdPtr->second)};
    // Check exists file.
    if (!QFile::exists(pathToFile)) {
      auto functionName{"getTextFromNote"};
      QString message{"Error in function `%1`. File `%2` not exists."};
      message = message.arg(functionName).arg(pathToFile);
      std::runtime_error(message.toStdString());
    }

    QFile file(getPathToText(findNoteIdPtr->second));
    file.open(QIODevice::ReadOnly);
    result = QString(file.readAll());
    file.close();
  }

  return result;
}

void Storage::addNotesFromStorage(QListWidget &widget, const QIcon &icon) {
  auto notes{noteInform[jsConst.notes].toObject()};
  for (const auto &id : notes.keys()) {
    // Get information about note.
    auto noteObject{notes[id].toObject()};
    auto title{noteObject[jsConst.noteTitle].toString()};
    // Create new note and add it.
    auto notePtr = new QListWidgetItem(icon, title, &widget);
    noteIds.insert({notePtr, id});
  }
}

QList<QListWidgetItem *> Storage::getNotesPointers() {
  QList<QListWidgetItem *> result;
  for (const auto &item : noteIds) {
    result.push_back(item.first);
  }
  return result;
}
