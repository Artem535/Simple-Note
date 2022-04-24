#pragma once
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QListWidgetItem>
#include <QString>
#include <stdexcept>
#include <map>
#include <stdexcept>

struct Paths {
  QString workDir;
  QString notesDir;
  QString inform;
};

struct JsonConstants {
  const QString freeId;
  const QString notes;
  const QString noteTitle;
  const QString pathToFile;
  JsonConstants()
      : freeId("free id"), notes("notes"), noteTitle("title"),
        pathToFile("file") {}
};

/**
 * @brief The Storage class
 */
class Storage {
  // TODO: Change it. Create own QListWidgetItem and QListWidget.
  Paths paths;
  JsonConstants jsConst;
  std::map<QListWidgetItem *, QString> noteIds;
  QJsonObject noteInform;
  QString nameStorageDir;

  // TODO: Write coments.
  /**
   * @brief Read informations about notes.
   * @param pathToFile
   */
  void readNotesInformation(const QString &pathToFile);

  /**
   * @brief Save information about notes in `inform.json`.
   */
  void saveNotesInformation();

  /**
   * @brief Get the Storage Directory object.
   * @return QString -- The path to the directory where all files are located.
   */
  QString getStorageDirectory();

  /**
   * @brief Create a First Init Files object.
   * Create `inform.json`, `notes` directory and note.
   */
  void createFirstInitFiles();

  /**
   * @brief Update information about notes.
   * @param id -- Id note.
   * @param title -- Title note.
   */
  void addNoteInInformFile(const QString &id, const QString &title);

  /**
   * @brief Save text note in file.
   * @param id -- Id note.
   * @param text -- Text of the note.
   */
  void saveNoteTextFile(const QString &id, const QString &text);

  /**
   * @brief Get the Path To Text object.
   * @param id -- Id note.
   * @return QString -- Path to text file for note with special id.
   */
  QString getPathToText(const QString &id);

public:
  /**
   * @brief Construct a new Storage object.
   */
  Storage();

  /**
   * @brief Get the Free Id.
   * @return QString -- A free id that can be used.
   */
  QString getFreeId();

  /**
   * @brief Get the Notes Titles object.
   * @return QList<QString> -- All note titles in storage.
   */
  QList<QString> getNotesTitles();

  /**
   * @brief Remove note from storage.
   * Function remove all information about note from `inform.json`.
   * Also remove file with text for note.
   * @param notePtr -- Pointer on note in QListWidget.
   */
  void removeNote(QListWidgetItem *notePtr);

  /**
   * @brief Add note in storage.
   * @param notePtr -- Pointer on item in QListWidget.
   * @param title -- Title of the note.
   * @param text -- Text of the note.
   */
  void addNote(QListWidgetItem *notePtr, const QString &title,
               const QString &text);

  /**
   * @brief Change title of the note in storage.
   * @param notePtr -- Pointer on note in QListWidget.
   * @param newTitle -- New title of the note.
   */
  void renameNote(QListWidgetItem *notePtr, const QString &newTitle);

  /**
   * @brief Change text of the note.
   * @param notePtr -- Pointer on note in QListWidget.
   * @param newText -- New text of the note.
   */
  void changeNoteText(QListWidgetItem *notePtr, const QString &newText);

  /**
   * @brief Get the Text From Note object.
   * @param notePtr -- Pointer on note in QListWidget.
   * @return QString -- Text of the note.
   */
  QString getTextFromNote(QListWidgetItem *notePtr);

  /**
   * @brief Read information about notes from storage,
   * and add it QListWidget. Also add icon to QListWidgetItem.
   * @param widget
   * @param icon
   */
  void addNotesFromStorage(QListWidget &widget, const QIcon &icon);

  /**
   * @brief Get the Notes Pointers object
   * @return QList<QListWidgetItem *>
   */
  QList<QListWidgetItem *> getNotesPointers();
};
