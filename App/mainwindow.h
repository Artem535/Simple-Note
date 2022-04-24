#pragma once

#include "storage.h"
#include <QApplication>
#include <QBitmap>
#include <QFontDatabase>
#include <QIcon>
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QSvgRenderer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @brief The Style struct
 */
struct Style {
  QIcon noteIcon;
  QIcon notePlusIcon;
  QIcon noteSaveIcon;
  QIcon noteRemoveIcon;
  QString newNoteName;
  QChar notSavedMark;

  Style() : newNoteName("New note"), notSavedMark('*') {
    QPixmap notePlusIcon{
        svgToPixmap({512, 512}, ":res/icons/journal-plus.svg")};
    QPixmap noteIcon{svgToPixmap({512, 512}, ":/res/icons/journal-text.svg")};
    QPixmap noteSaveIcon{
        svgToPixmap({512, 512}, ":/res/icons/journal-check.svg")};
    QPixmap noteRemoveIcon{
        svgToPixmap({512, 512}, ":/res/icons/journal-x.svg")};

    auto textColor{QApplication::palette().text().color()};
    changeColor(notePlusIcon, textColor);
    changeColor(noteIcon, textColor);
    changeColor(noteSaveIcon, textColor);
    changeColor(noteRemoveIcon, textColor);

    this->noteIcon.addPixmap(noteIcon);
    this->notePlusIcon.addPixmap(notePlusIcon);
    this->noteSaveIcon.addPixmap(noteSaveIcon);
    this->noteRemoveIcon.addPixmap(noteRemoveIcon);
  }

  /**
   * @brief changeColor
   * Function change color icons.
   * Create mask, fill pixmap, apply mask.
   * @param img
   * @param color
   * @todo Find better way. It's slow function and not a very good result.
   */
  void changeColor(QPixmap &img, const QColor &color) {
    auto mask{img.createMaskFromColor(Qt::GlobalColor::transparent)};
    img.fill(color);
    img.setMask(mask);
  }

  QPixmap svgToPixmap(const QSize &imgSize, const QString &path) {
    QSvgRenderer render(path);
    QPixmap img(imgSize);
    img.fill(Qt::GlobalColor::transparent);

    QPainter painter;
    painter.begin(&img);
    render.render(&painter);
    painter.end();

    return img;
  }
};

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

  /**
   * @brief clearScreen
   */
  void clearScreen();

  /**
   * @brief textSearchLineEdited
   * @param newText
   */
  void textSearchLineEdited(const QString &newText);

private:
  Storage storage;
  Ui::MainWindow *ui;
  const Style style;
  enum screenState { note = 0, clear };
};
