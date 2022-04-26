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
#include <QSpinBox>
#include <QSvgRenderer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct Constans {
  const QChar notSavedMark;
  const QString newNoteName;
  Constans() : notSavedMark('*'), newNoteName("New note.") {}
};

/**
 * @brief The Style struct
 */
struct Style {
  QIcon noteIcon;
  QIcon notePlusIcon;
  QIcon noteSaveIcon;
  QIcon noteRemoveIcon;

  Style() {
    QPixmap notePlusIcon{
        svgToPixmap({512, 512}, "://res/icons/journal-plus.svg")};
    QPixmap noteIcon{svgToPixmap({512, 512}, "://res/icons/journal-text.svg")};
    QPixmap noteSaveIcon{
        svgToPixmap({512, 512}, "://res/icons/journal-check.svg")};
    QPixmap noteRemoveIcon{
        svgToPixmap({512, 512}, "://res/icons/journal-x.svg")};

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
   * @brief toggleSaveMark.
   * Add or remove to text QListWidgetItem mark `notSaved`.
   * @param notePtr
   */
  void toggleSaveMark(QListWidgetItem *notePtr, bool addMark = false);

  /**
   * @brief toggleSaveMark.
   * Add or remove to string mark `notSaved`.
   * @param str
   */
  void toggleSaveMark(QString &str, bool addMark = false);

  /**
   * @brief showErrorMessage.
   * Show window with error icon and specific text.
   * @param text -- Text error message.
   */
  void showErrorMessage(const QString &text);

public slots:
  /**
   * @brief createNewNote.
   * Create new note.
   */
  void createNewNote();

  /**
   * @brief displayNote.
   * Dispaly text and title note.
   * @param item
   */
  void displayNote(QListWidgetItem *item);

  /**
   * @brief saveNote.
   * Called when the save button is clicked.
   * Save title and text into `storage`.
   */
  void saveNoteButtonPush();

  /**
   * @brief deleteNote.
   * Remove current QListWidgetItem from storage and QListWidget.
   */
  void removeNoteButtonPush();

  /**
   * @brief clearScreen.
   * Show 'clear' screen and remove text from ui->textNote, ui->titleNote.
   */
  void clearScreen();

  /**
   * @brief textSearchLineEdited. Filters QListWidget.
   * Shows only those QListWidgetItem in the `text` of which contains string
   * `filter`.
   * @param newText
   */
  void textSearchLineEdited(const QString &filter);

  /**
   * @brief editButtonPush.
   * Convert text from markDown to plainText.
   * Disable `readOnly` from `ui->titleNote` and `ui->textNote`.
   */
  void editButtonPush();

  /**
   * @brief noteReadOnly.
   * Make note only for read.
   * Sets the property `readOnly` to `ui->titleNote` and `ui->textNote`.
   * @param readOnly -- State.
   */
  void noteReadOnly(bool readOnly);

  /**
   * @brief fontNoteChanged.
   * Change current note in `ui->textNote`.
   * @param font -- New font.
   */
  void fontNoteChanged(const QFont &font);

  /**
   * @brief fontNoteSizeChanged
   * int i -- New size font.
   */
  void fontNoteSizeChanged(int newSize);

  /**
   * @brief textNoteChanged
   * The function is called to change the `text` of the current QListWidgetItem.
   * It is marked as not being saved.
   */
  void textNoteChanged();

private:
  Storage storage;
  Ui::MainWindow *ui;
  const Style style;
  Constans constants;
  enum screenState { note = 0, clear };
};
