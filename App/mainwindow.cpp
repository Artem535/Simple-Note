#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  // Create new buttons(`add new note`) to toolbar.
  ui->toolBar->addAction(style.notePlusIcon, "Add new note.", this,
                         &MainWindow::createNewNote);

  // Load information from storage about notes.
  storage.addNotesFromStorage(*ui->listWidget, style.noteIcon);
  // Connect buttons.
  connect(ui->saveButton, &QAbstractButton::clicked, this,
          &MainWindow::saveNote);
  connect(ui->deleteButton, &QAbstractButton::clicked, this,
          &MainWindow::deleteNote);
  // Connect actions.
  connect(ui->listWidget, &QListWidget::itemClicked, this,
          &MainWindow::displayNote);
  connect(ui->searchLine, &QLineEdit::textChanged, this,
          &MainWindow::textSearchLineEdited);
  // Hide tab bar.
  ui->tabWidget->tabBar()->hide();
  // Add font to applications.
  QFontDatabase::addApplicationFont(":/res/fonts/Inter-Regular.ttf");
  // Set default screen.
  ui->tabWidget->setCurrentIndex(screenState::clear);
  // Add icons to button.
  ui->saveButton->setIcon(style.noteSaveIcon);
  ui->deleteButton->setIcon(style.noteRemoveIcon);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::toggleSaveMark(QListWidgetItem *notePtr, bool addMark) {
  auto title{notePtr->text()};
  toggleSaveMark(title, addMark);
  notePtr->setText(title);
}

void MainWindow::toggleSaveMark(QString &str, bool addMark) {
  // All note title with last symbal equal `SAVE_MARK` marked as not saved.
  if (str.back() == style.notSavedMark && !addMark) {
    str.chop(1);
  } else if (addMark) {
    str.push_back(style.notSavedMark);
  }
}

void MainWindow::createNewNote() {
  // Create and add icon.
  auto newNotePtr =
      new QListWidgetItem(style.noteIcon, "New note", ui->listWidget);

  // Add in storage and display.
  toggleSaveMark(newNotePtr, true);
  ui->listWidget->setCurrentItem(newNotePtr);
  displayNote(newNotePtr);
}

void MainWindow::displayNote(QListWidgetItem *item) {
  ui->tabWidget->setCurrentIndex(screenState::note);
  auto title{item->text()};
  toggleSaveMark(title);
  ui->titleNote->setText(title);

  auto text{storage.getTextFromNote(item)};
  ui->textNote->setHtml(text);
}

void MainWindow::saveNote() {
  auto notePtr = ui->listWidget->currentItem();
  // notePtr can be null ptr.
  if (notePtr != nullptr) {
    notePtr->setText(ui->titleNote->text());
    storage.addNote(notePtr, ui->titleNote->text(), ui->textNote->toHtml());
  }
}

void MainWindow::deleteNote() {
  auto reply = QMessageBox::question(
      this, "Delete note", "Are you sure you want to delete this note?",
      QMessageBox::Yes | QMessageBox::No);

  if (reply == QMessageBox::Yes) {
    auto selectedRow = ui->listWidget->currentRow();
    // Remove from storage.
    storage.removeNote(ui->listWidget->currentItem());
    // Remove from ListWidgets.
    ui->listWidget->removeItemWidget(ui->listWidget->takeItem(selectedRow));
    // Show empty screen.
    ui->tabWidget->setCurrentIndex(screenState::clear);
  }
}

void MainWindow::clearScreen() {
  ui->titleNote->clear();
  ui->textNote->clear();
}

void MainWindow::textSearchLineEdited(const QString &newText) {
  auto findedItems{ui->listWidget->findItems(newText, Qt::MatchContains)};
  for (auto items : storage.getNotesPointers()) {
    if (!findedItems.contains(items)) {
      items->setHidden(true);
    } else {
      items->setHidden(false);
    }
  }
}
