// MainWindow.cpp — Main window implementation: matrix grids, operations, result display, error handling.

#include "MainWindow.hpp"
#include "fraction.hpp"
#include <QApplication>
#include <QDebug>
#include <QLineEdit>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QStatusBar>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <Qt>

namespace {
  const int kMaxRowsCols = 20;
  const int kDefaultRows = 2;
  const int kDefaultCols = 2;
}

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , rowsA_(nullptr)
  , colsA_(nullptr)
  , tableA_(nullptr)
  , rowsB_(nullptr)
  , colsB_(nullptr)
  , tableB_(nullptr)
  , resultTable_(nullptr)
  , scalarEdit_(nullptr)
  , statusBar_(nullptr)
  , centralWidget_(nullptr)
{
  setWindowTitle(tr("Matrix Calculator"));
  setMinimumSize(900, 600);
  resize(1000, 700);
  setupUi();
  runInternalTests();
  showStatus(tr("Ready. Edit matrices and choose an operation."));
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi() {
  centralWidget_ = new QWidget(this);
  setCentralWidget(centralWidget_);
  QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget_);
  mainLayout->setSpacing(12);
  mainLayout->setContentsMargins(12, 12, 12, 12);

  buildMatrixInputs();
  buildOperationPanel();
  buildResultView();

  statusBar_ = new QStatusBar(this);
  setStatusBar(statusBar_);

  // Prefill with example 2×2 matrices for demonstration
  if (tableA_->rowCount() >= 2 && tableA_->columnCount() >= 2) {
    tableA_->item(0, 0)->setText(QString::number(1));
    tableA_->item(0, 1)->setText(QString::number(2));
    tableA_->item(1, 0)->setText(QString::number(3));
    tableA_->item(1, 1)->setText(QString::number(4));
  }
  if (tableB_->rowCount() >= 2 && tableB_->columnCount() >= 2) {
    tableB_->item(0, 0)->setText(QString::number(1));
    tableB_->item(0, 1)->setText(QString::number(0));
    tableB_->item(1, 0)->setText(QString::number(0));
    tableB_->item(1, 1)->setText(QString::number(1));
  }
}

void MainWindow::buildMatrixInputs() {
  QHBoxLayout* matricesLayout = new QHBoxLayout();
  matricesLayout->setSpacing(24);

  auto makeMatrixGroup = [this](const QString& title, QSpinBox*& rows, QSpinBox*& cols, QTableWidget*& table) {
    QGroupBox* g = new QGroupBox(title);
    QVBoxLayout* v = new QVBoxLayout(g);
    QFormLayout* form = new QFormLayout();
    rows = new QSpinBox();
    rows->setRange(1, kMaxRowsCols);
    rows->setValue(kDefaultRows);
    cols = new QSpinBox();
    cols->setRange(1, kMaxRowsCols);
    cols->setValue(kDefaultCols);
    form->addRow(tr("Rows:"), rows);
    form->addRow(tr("Cols:"), cols);
    v->addLayout(form);
    table = new QTableWidget();
    table->setMinimumSize(180, 120);
    connect(rows, &QSpinBox::valueChanged, this, &MainWindow::onRowsColsChanged);
    connect(cols, &QSpinBox::valueChanged, this, &MainWindow::onRowsColsChanged);
    v->addWidget(table);
    return g;
  };

  QGroupBox* groupA = makeMatrixGroup(tr("Matrix A"), rowsA_, colsA_, tableA_);
  QGroupBox* groupB = makeMatrixGroup(tr("Matrix B"), rowsB_, colsB_, tableB_);
  matricesLayout->addWidget(groupA);
  matricesLayout->addWidget(groupB);

  onRowsColsChanged();

  static_cast<QVBoxLayout*>(centralWidget_->layout())->addLayout(matricesLayout);
}

void MainWindow::onRowsColsChanged() {
  const int rA = rowsA_->value();
  const int cA = colsA_->value();
  const int rB = rowsB_->value();
  const int cB = colsB_->value();
  tableA_->setRowCount(rA);
  tableA_->setColumnCount(cA);
  tableB_->setRowCount(rB);
  tableB_->setColumnCount(cB);
  for (int i = 0; i < rA; ++i)
    for (int j = 0; j < cA; ++j) {
      if (!tableA_->item(i, j)) {
        tableA_->setItem(i, j, new QTableWidgetItem(QString::number(0)));
      }
    }
  for (int i = 0; i < rB; ++i)
    for (int j = 0; j < cB; ++j) {
      if (!tableB_->item(i, j)) {
        tableB_->setItem(i, j, new QTableWidgetItem(QString::number(0)));
      }
    }
}

void MainWindow::buildOperationPanel() {
  QGroupBox* opsGroup = new QGroupBox(tr("Operations"));
  QVBoxLayout* v = new QVBoxLayout(opsGroup);
  scalarEdit_ = new QLineEdit();
  scalarEdit_->setPlaceholderText(tr("e.g. 2, 1/2, -3/4"));
  scalarEdit_->setText(QStringLiteral("1"));
  QFormLayout* scalarForm = new QFormLayout();
  scalarForm->addRow(tr("Scalar (fraction or number):"), scalarEdit_);
  v->addLayout(scalarForm);

  QGridLayout* grid = new QGridLayout();
  int row = 0;
  auto addBtn = [&grid, &row, this](const QString& text, void (MainWindow::*slot)()) {
    QPushButton* btn = new QPushButton(text);
    connect(btn, &QPushButton::clicked, this, slot);
    grid->addWidget(btn, row / 2, row % 2);
    ++row;
  };
  addBtn(tr("A + B"), &MainWindow::performAddition);
  addBtn(tr("A − B"), &MainWindow::performSubtractionAB);
  addBtn(tr("B − A"), &MainWindow::performSubtractionBA);
  addBtn(tr("A × B"), &MainWindow::performMultiplyAB);
  addBtn(tr("B × A"), &MainWindow::performMultiplyBA);
  addBtn(tr("A × scalar"), &MainWindow::performScalarMultiplyA);
  addBtn(tr("B × scalar"), &MainWindow::performScalarMultiplyB);
  addBtn(tr("A / scalar"), &MainWindow::performScalarDivideA);
  addBtn(tr("B / scalar"), &MainWindow::performScalarDivideB);
  addBtn(tr("RREF(A)"), &MainWindow::performRREFOnA);
  addBtn(tr("RREF(B)"), &MainWindow::performRREFOnB);
  addBtn(tr("Inverse(A)"), &MainWindow::performInverseA);
  addBtn(tr("Inverse(B)"), &MainWindow::performInverseB);
  v->addLayout(grid);

  static_cast<QVBoxLayout*>(centralWidget_->layout())->addWidget(opsGroup);
}

void MainWindow::buildResultView() {
  QGroupBox* resultGroup = new QGroupBox(tr("Result"));
  QVBoxLayout* v = new QVBoxLayout(resultGroup);
  resultTable_ = new QTableWidget();
  resultTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
  resultTable_->setMinimumSize(200, 120);
  v->addWidget(resultTable_);
  static_cast<QVBoxLayout*>(centralWidget_->layout())->addWidget(resultGroup);
}

Matrix MainWindow::loadMatrixFromTable(QTableWidget* table) const {
  const int r = table->rowCount();
  const int c = table->columnCount();
  Matrix M(static_cast<std::size_t>(r), static_cast<std::size_t>(c));
  for (int i = 0; i < r; ++i)
    for (int j = 0; j < c; ++j) {
      QTableWidgetItem* item = table->item(i, j);
      Fraction val(0, 1);
      if (item && !item->text().trimmed().isEmpty())
        val = Fraction::fromString(item->text().trimmed().toStdString());
      M(static_cast<std::size_t>(i), static_cast<std::size_t>(j)) = val;
    }
  return M;
}

void MainWindow::displayMatrixInTable(const Matrix& M, QTableWidget* table) {
  table->setRowCount(static_cast<int>(M.rows()));
  table->setColumnCount(static_cast<int>(M.cols()));
  for (std::size_t i = 0; i < M.rows(); ++i)
    for (std::size_t j = 0; j < M.cols(); ++j) {
      QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(M(i, j).toString()));
      table->setItem(static_cast<int>(i), static_cast<int>(j), item);
    }
}

void MainWindow::setResult(const Matrix& M) {
  displayMatrixInTable(M, resultTable_);
  showStatus(tr("Result updated."));
}

void MainWindow::showError(const QString& message) {
  showStatus(message);
  QMessageBox::warning(this, tr("Error"), message);
}

void MainWindow::showStatus(const QString& message) {
  if (statusBar_) statusBar_->showMessage(message);
}

void MainWindow::performAddition() {
  try {
    Matrix A = loadMatrixFromTable(tableA_);
    Matrix B = loadMatrixFromTable(tableB_);
    setResult(A + B);
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performSubtractionAB() {
  try {
    Matrix A = loadMatrixFromTable(tableA_);
    Matrix B = loadMatrixFromTable(tableB_);
    setResult(A - B);
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performSubtractionBA() {
  try {
    Matrix A = loadMatrixFromTable(tableA_);
    Matrix B = loadMatrixFromTable(tableB_);
    setResult(B - A);
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performMultiplyAB() {
  try {
    Matrix A = loadMatrixFromTable(tableA_);
    Matrix B = loadMatrixFromTable(tableB_);
    setResult(A * B);
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performMultiplyBA() {
  try {
    Matrix A = loadMatrixFromTable(tableA_);
    Matrix B = loadMatrixFromTable(tableB_);
    setResult(B * A);
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performScalarMultiplyA() {
  try {
    Matrix A = loadMatrixFromTable(tableA_);
    Fraction s = Fraction::fromString(scalarEdit_->text().trimmed().toStdString());
    setResult(A * s);
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performScalarMultiplyB() {
  try {
    Matrix B = loadMatrixFromTable(tableB_);
    Fraction s = Fraction::fromString(scalarEdit_->text().trimmed().toStdString());
    setResult(B * s);
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performScalarDivideA() {
  try {
    Matrix A = loadMatrixFromTable(tableA_);
    Fraction s = Fraction::fromString(scalarEdit_->text().trimmed().toStdString());
    setResult(A / s);
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performScalarDivideB() {
  try {
    Matrix B = loadMatrixFromTable(tableB_);
    Fraction s = Fraction::fromString(scalarEdit_->text().trimmed().toStdString());
    setResult(B / s);
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performRREFOnA() {
  try {
    Matrix A = loadMatrixFromTable(tableA_);
    setResult(A.rref());
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performRREFOnB() {
  try {
    Matrix B = loadMatrixFromTable(tableB_);
    setResult(B.rref());
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performInverseA() {
  try {
    Matrix A = loadMatrixFromTable(tableA_);
    setResult(A.inverse());
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::performInverseB() {
  try {
    Matrix B = loadMatrixFromTable(tableB_);
    setResult(B.inverse());
  } catch (const std::exception& e) {
    showError(QString::fromUtf8(e.what()));
  }
}

void MainWindow::runInternalTests() {
  auto run = [this](const char* name, bool ok) {
    if (ok)
      qDebug("Matrix test OK: %s", name);
    else
      qDebug("Matrix test FAILED: %s", name);
  };
  try {
    Matrix A(2, 2);
    A(0, 0) = Fraction(1, 1); A(0, 1) = Fraction(2, 1);
    A(1, 0) = Fraction(3, 1); A(1, 1) = Fraction(4, 1);
    Matrix B(2, 2);
    B(0, 0) = Fraction(5, 1); B(0, 1) = Fraction(6, 1);
    B(1, 0) = Fraction(7, 1); B(1, 1) = Fraction(8, 1);
    Matrix sum = A + B;
    Matrix diff = sum - B;
    run("(A+B)-B ≈ A", Matrix::approxEqual(diff, A));

    Matrix I(2, 2);
    I(0, 0) = Fraction(1, 1); I(0, 1) = Fraction(0, 1);
    I(1, 0) = Fraction(0, 1); I(1, 1) = Fraction(1, 1);
    Matrix AI = A * I;
    run("A*I ≈ A", Matrix::approxEqual(AI, A));

    Matrix C(3, 3);
    C(0, 0) = Fraction(1, 1); C(0, 1) = Fraction(0, 1); C(0, 2) = Fraction(1, 1);
    C(1, 0) = Fraction(0, 1); C(1, 1) = Fraction(2, 1); C(1, 2) = Fraction(0, 1);
    C(2, 0) = Fraction(1, 1); C(2, 1) = Fraction(0, 1); C(2, 2) = Fraction(2, 1);
    Matrix invC = C.inverse();
    Matrix CinvC = C * invC;
    Matrix I3(3, 3);
    I3(0, 0) = Fraction(1, 1); I3(0, 1) = Fraction(0, 1); I3(0, 2) = Fraction(0, 1);
    I3(1, 0) = Fraction(0, 1); I3(1, 1) = Fraction(1, 1); I3(1, 2) = Fraction(0, 1);
    I3(2, 0) = Fraction(0, 1); I3(2, 1) = Fraction(0, 1); I3(2, 2) = Fraction(1, 1);
    run("A*A.inverse() ≈ I (3x3)", Matrix::approxEqual(CinvC, I3));
  } catch (const std::exception& e) {
    qDebug("Matrix internal test exception: %s", e.what());
  }
}