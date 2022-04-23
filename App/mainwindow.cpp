#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), NOT_SAVE_MARK('*') {
  ui->setupUi(this);
  // Create new buttons(`add new note`) to toolbar.
  ui->toolBar->addAction("a", this, &MainWindow::createNewNote);
  // Load information from storage about notes.
  storage.addNotesFromStorage(*ui->listWidget);
  // Connect buttons.
  connect(ui->saveButton, &QAbstractButton::clicked, this,
          &MainWindow::saveNote);
  connect(ui->listWidget, &QListWidget::itemClicked, this,
          &MainWindow::displayNote);
  connect(ui->deleteButton, &QAbstractButton::clicked, this,
          &MainWindow::deleteNote);
  // Set clear screen.
  ui->tabWidget->setCurrentIndex(1);
  // Hide tab bar.
  ui->tabWidget->tabBar()->hide();
  // Change style.
  //  {
  //    QFile file(":/res/style/MaterialDark.qss");
  //    file.open(QIODevice::ReadOnly);
  //    QString style{file.readAll()};
  //    file.close();
  //    MainWindow::setStyleSheet(style);
  //  }
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::toggleSaveMark(QListWidgetItem *notePtr, bool addMark) {
  auto title{notePtr->text()};
  toggleSaveMark(title, addMark);
  notePtr->setText(title);
}

void MainWindow::toggleSaveMark(QString &str, bool addMark) {
  // All note title with last symbal equal `SAVE_MARK` marked as not saved.
  if (str.back() == NOT_SAVE_MARK && !addMark) {
    str.chop(1);
  } else if (addMark) {
    str.push_back(NOT_SAVE_MARK);
  }
}

void MainWindow::createNewNote() {
  auto newNotePtr = new QListWidgetItem("New note", ui->listWidget);
  toggleSaveMark(newNotePtr, true);
  ui->listWidget->setCurrentItem(newNotePtr);
  displayNote(newNotePtr);
}

void MainWindow::displayNote(QListWidgetItem *item) {
  ui->tabWidget->setCurrentIndex(0);
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
  }
}

void MainWindow::clearScreen() {
  ui->titleNote->clear();
  ui->textNote->clear();
}
