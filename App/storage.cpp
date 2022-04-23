#include "storage.h"

void Storage::readNotesInformation(const QString &pathToFile) {
  QFile file(pathToFile);
  file.open(QIODevice::ReadOnly);
  noteInform = QJsonDocument::fromJson(file.readAll()).object();
  file.close();
}

void Storage::saveNotesInformation() {
  QFile file(notesDirectory + "/inform.json");
  file.open(QIODevice::WriteOnly);
  QJsonDocument outputDoc;
  outputDoc.setObject(noteInform);
  file.write(outputDoc.toJson());
  file.close();
}

QString Storage::getStorageDirectory() {
  auto directory{QDir::home()};
  bool newFolderCreate = directory.mkdir(".SimpleNotes");
  directory.cd(".SimpleNotes");

  if (newFolderCreate) {
    createFirstInitFiles(directory.path());
  }

  return directory.path();
}

void Storage::createFirstInitFiles(const QString &path) {
  // Create notes informations.
  QFile file{path + "/inform.json"};
  file.open(QIODevice::WriteOnly);
  const QString emptyTemplate{"{\"free id\": [0], \"notes\": {}}"};
  file.write(emptyTemplate.toStdString().c_str());
  file.close();

  // Create directory for text notes.
  QDir directory{path};
  directory.mkdir("notes");
}

void Storage::addNoteInInformFile(const unsigned int &id,
                                  const QString &title) {
  // Create `value`: {"path to file": ..., "title": ...}
  QJsonObject value;
  value.insert("path to file", notesDirectory + "/" + QString::number(id));
  value.insert("title", title);
  // Qt can't change value 'complex' object.
  // We needed get object, insert new object in array, then replace `notes`
  // object.
  auto notesObject{noteInform["notes"].toObject()};
  notesObject.insert(QString::number(id), value);
  noteInform["notes"] = notesObject;
}

void Storage::saveNoteTextFile(const unsigned int &id, const QString &text) {
  QFile file(notesDirectory + "/notes/" + QString::number(id));
  file.open(QIODevice::WriteOnly);
  file.write(text.toStdString().c_str());
  file.close();
}

Storage::Storage() {
  auto dirPath{getStorageDirectory()};
  readNotesInformation(dirPath + "/inform.json");
  notesDirectory = dirPath;
}

unsigned int Storage::getFreeId() {
  // From `value` we get QJsonValue, convert it to array.
  // Get first element array and convert it to int.
  auto array{noteInform.value("free id").toArray()};
  int nedeedId{0};
  if (array.empty()) {
    nedeedId = noteInform["notes"].toObject().size();
  } else {
    nedeedId = array.first().toInt();
    array.pop_front();
    // Then replace `free id` array.
    noteInform["free id"] = array;
  }
  return nedeedId;
}

void Storage::removeNote(QListWidgetItem *notePtr) {
  auto findedNote{noteIds.find(notePtr)};
  bool noteExsist = findedNote != noteIds.end();
  if (noteExsist) {
    auto id{QString::number(findedNote->second)};
    // Remove text note
    QFile::remove(notesDirectory + "/notes/" + id);
    // Remove information abou note.
    auto notesObject{noteInform["notes"].toObject()};
    notesObject.remove(id);
    noteInform["notes"] = notesObject;
    // Add `id` in array with free id.
    auto freeIdArray{noteInform["free id"].toArray()};
    freeIdArray.push_back(id.toInt());
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
  auto id{QString::number(noteIds[notePtr])};
  auto notesObject{noteInform["notes"].toObject()};
  auto idObject{notesObject[id].toObject()};

  idObject["title"] = newTitle;
  notesObject[id] = idObject;
  noteInform["notes"] = notesObject;
}

void Storage::changeNoteText(QListWidgetItem *notePtr, const QString &newText) {
  auto findedNote{noteIds.find(notePtr)};
  if (findedNote != noteIds.end()) {
    QFile file(notesDirectory + "/notes/" +
               QString::number(findedNote->second));
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
    QFile file(notesDirectory + "/notes/" +
               QString::number(findNoteIdPtr->second));
    file.open(QIODevice::ReadOnly);
    result = QString(file.readAll());
    file.close();
  }

  return result;
}

void Storage::addNotesFromStorage(QListWidget &widget) {
  auto notes{noteInform["notes"].toObject()};
  for (const auto &noteKey : notes.keys()) {
    // Get inforamtion about note.
    auto id{noteKey.toUInt()};
    auto noteObject{notes[noteKey].toObject()};
    auto title{noteObject["title"].toString()};
    // Create new note and add it.
    auto notePtr = new QListWidgetItem(title, &widget);
    noteIds.insert({notePtr, id});
  }
}
