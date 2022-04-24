#pragma once
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QListWidgetItem>
#include <QString>
#include <map>

/**
 * @brief The Storage class
 */
class Storage {
  // TODO: Change it. Create own QListWidgetItem and QListWidget.
  std::map<QListWidgetItem *, unsigned int> noteIds;
  QJsonObject noteInform;
  QString notesDirectory;

  // TODO: Write coments.
  void readNotesInformation(const QString &pathToFile);
  void saveNotesInformation();
  QString getStorageDirectory();
  void createFirstInitFiles(const QString &path);
  void addNoteInInformFile(const unsigned int &id, const QString &title);
  void saveNoteTextFile(const unsigned int &id, const QString &text);

public:
  /**
   * @brief Storage
   */
  Storage();

  /**
   * @brief getId
   * @param notePtr
   * @return
   */
  unsigned int getFreeId();

  /**
   * @brief getNotesTitles
   * @return
   */
  QList<QString> getNotesTitles();

  /**
   * @brief removeNotes
   * @param id
   */
  void removeNote(QListWidgetItem *notePtr);

  /**
   * @brief addNote
   * @param notePtr
   * @param title
   * @param text
   */
  void addNote(QListWidgetItem *notePtr, const QString &title,
               const QString &text);

  /**
   * @brief renameNote
   * @param id
   * @param newTitle
   */
  void renameNote(QListWidgetItem *notePtr, const QString &newTitle);

  /**
   * @brief changeNoteText
   * @param id
   * @param newText
   */
  void changeNoteText(QListWidgetItem *notePtr, const QString &newText);

  /**
   * @brief getTextFromNote
   * @param id
   * @return
   */
  QString getTextFromNote(QListWidgetItem *notePtr);

  /**
   * @brief addNotesFromStorage
   * @param widget
   */
  void addNotesFromStorage(QListWidget &widget, const QIcon &icon);

  QList<QListWidgetItem *> getNotesPointers();
};
