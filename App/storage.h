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
   * @brief 
   * 
   * @param pathToFile 
   */
  void readNotesInformation(const QString &pathToFile);
  
  /**
   * @brief 
   * 
   */
  void saveNotesInformation();
  
  /**
   * @brief Get the Storage Directory object
   * 
   * @return QString 
   */
  QString getStorageDirectory();
  
  /**
   * @brief Create a First Init Files object
   * 
   */
  void createFirstInitFiles();
  
  /**
   * @brief 
   * 
   * @param id 
   * @param title 
   */
  void addNoteInInformFile(const QString &id, const QString &title);
  
  /**
   * @brief 
   * 
   * @param id 
   * @param text 
   */
  void saveNoteTextFile(const QString &id, const QString &text);
  
  /**
   * @brief Get the Path To Text object
   * 
   * @param id 
   * @return QString 
   */
  QString getPathToText(const QString &id);
  
public:
  /**
   * @brief Construct a new Storage object
   * 
   */
  Storage();

 /**
  * @brief Get the Free Id object
  * 
  * @return unsigned int 
  */
  QString getFreeId();

 /**
  * @brief Get the Notes Titles object
  * 
  * @return QList<QString> 
  */
  QList<QString> getNotesTitles();

  /**
   * @brief 
   * 
   * @param notePtr 
   */
  void removeNote(QListWidgetItem *notePtr);

  /**
   * @brief 
   * 
   * @param notePtr 
   * @param title 
   * @param text 
   */
  void addNote(QListWidgetItem *notePtr, const QString &title,
               const QString &text);

  /**
   * @brief 
   * 
   * @param notePtr 
   * @param newTitle 
   */
  void renameNote(QListWidgetItem *notePtr, const QString &newTitle);

  /**
   * @brief 
   * 
   * @param notePtr 
   * @param newText 
   */
  void changeNoteText(QListWidgetItem *notePtr, const QString &newText);

  /**
   * @brief Get the Text From Note object
   * 
   * @param notePtr 
   * @return QString 
   */
  QString getTextFromNote(QListWidgetItem *notePtr);

  /**
   * @brief 
   * 
   * @param widget 
   * @param icon 
   */
  void addNotesFromStorage(QListWidget &widget, const QIcon &icon);
  
  /**
   * @brief Get the Notes Pointers object
   * 
   * @return QList<QListWidgetItem *> 
   */
  QList<QListWidgetItem *> getNotesPointers();
};
