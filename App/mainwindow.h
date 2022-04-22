#pragma once

#include "storage.h"
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  /**
   * @brief toggleSaveMark
   * @param notePtr
   */
  void toggleSaveMark(QListWidgetItem *notePtr, bool addMark = false);

  /**
   * @brief toggleSaveMark
   * @param str
   */
  void toggleSaveMark(QString &str, bool addMark = false);

public slots:
  /**
   * @brief createNewNote
   */
  void createNewNote();

  /**
   * @brief displayNote
   * @param item
   */
  void displayNote(QListWidgetItem *item);

  /**
   * @brief saveNote
   */
  void saveNote();

  /**
   * @brief deleteNote
   */
  void deleteNote();

  void clearScreen();

private:
  Storage storage;
  Ui::MainWindow *ui;
  const char NOT_SAVE_MARK;
};
