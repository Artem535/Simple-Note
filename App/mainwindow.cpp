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
          &MainWindow::saveNoteButtonPush);
  connect(ui->deleteButton, &QAbstractButton::clicked, this,
          &MainWindow::removeNoteButtonPush);
  connect(ui->editButton, &QAbstractButton::clicked, this,
          &MainWindow::editButtonPush);
  // Connect actions.
  connect(ui->listWidget, &QListWidget::itemClicked, this,
          &MainWindow::displayNote);
  connect(ui->searchLine, &QLineEdit::textChanged, this,
          &MainWindow::textSearchLineEdited);
  connect(ui->fontComboBox, &QFontComboBox::currentFontChanged, this,
          &MainWindow::fontNoteChanged);
  connect(ui->fontSizeBox, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::fontNoteSizeChanged);
  connect(ui->editButton, &QAbstractButton::clicked, this,
          &MainWindow::textNoteChanged);

  // Hide tab bar.
  ui->tabWidget->tabBar()->hide();
  // Add font to applications.
  QFontDatabase::addApplicationFont("://res/fonts/Inter-Regular.ttf");
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
  bool withSaveMark = str.back() == constants.notSavedMark;
  if (withSaveMark && !addMark) {
    str.chop(1);
  } else if (addMark && !withSaveMark) {
    str.push_back(constants.notSavedMark);
  }
}

void MainWindow::showErrorMessage(const QString &text) {
  QMessageBox box;
  box.setWindowTitle("Error!");
  box.setIcon(QMessageBox::Icon::Critical);
  box.setText(text);
  box.exec();
}

void MainWindow::createNewNote() {
  // Create and add icon.
  auto newNotePtr = new QListWidgetItem(style.noteIcon, constants.newNoteName,
                                        ui->listWidget);

  // Add in storage and display.
  toggleSaveMark(newNotePtr, true);
  ui->listWidget->setCurrentItem(newNotePtr);
  displayNote(newNotePtr);
}

void MainWindow::displayNote(QListWidgetItem *item) {
  // Set note only for read.
  noteReadOnly(true);
  // Change screen.
  ui->tabWidget->setCurrentIndex(screenState::note);
  // Set title.
  auto title{item->text()};
  toggleSaveMark(title);
  ui->titleNote->setText(title);
  // Set text.
  try {
    auto text{storage.getTextFromNote(item)};
    ui->textNote->setMarkdown(text);
  } catch (const std::runtime_error &error) {
    showErrorMessage(error.what());
  }
  // Set font size in spin box and font family in box.
}

void MainWindow::saveNoteButtonPush() {
  auto notePtr = ui->listWidget->currentItem();
  // notePtr can be null ptr.
  if (notePtr != nullptr) {
    notePtr->setText(ui->titleNote->text());
    try {
      storage.addNote(
          notePtr, ui->titleNote->text(),
          ui->textNote->toMarkdown(QTextDocument::MarkdownDialectCommonMark));
    } catch (const std::runtime_error &error) {
      showErrorMessage(error.what());
    }
  }
}

void MainWindow::removeNoteButtonPush() {
  auto reply = QMessageBox::question(
      this, "Delete note", "Are you sure you want to delete this note?",
      QMessageBox::Yes | QMessageBox::No);

  if (reply == QMessageBox::Yes) {
    auto selectedRow = ui->listWidget->currentRow();
    try {
      // Remove from storage.
      storage.removeNote(ui->listWidget->currentItem());
    } catch (const std::runtime_error &error) {
      showErrorMessage(error.what());
    }
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

void MainWindow::textSearchLineEdited(const QString &filter) {
  auto findedItems{ui->listWidget->findItems(filter, Qt::MatchContains)};
  for (auto items : storage.getNotesPointers()) {
    if (!findedItems.contains(items)) {
      items->setHidden(true);
    } else {
      items->setHidden(false);
    }
  }
}

void MainWindow::editButtonPush() {

  if (ui->editButton->isChecked()) {
    noteReadOnly(false);
    ui->textNote->setText(
        ui->textNote->toMarkdown(QTextDocument::MarkdownDialectCommonMark));
  } else {
    noteReadOnly(true);
    ui->textNote->setMarkdown(ui->textNote->toPlainText());
  }
}

void MainWindow::noteReadOnly(bool readOnly) {
  ui->titleNote->setReadOnly(readOnly);
  ui->textNote->setReadOnly(readOnly);
}

void MainWindow::fontNoteChanged(const QFont &font) {
  ui->textNote->setFont(font);
  ui->textNote->update();
}

void MainWindow::fontNoteSizeChanged(int newSize) {
  ui->textNote->selectAll();
  ui->textNote->setFontPointSize(newSize);
  ui->textNote->update();
}

void MainWindow::textNoteChanged() {
  toggleSaveMark(ui->listWidget->currentItem(), true);
}
