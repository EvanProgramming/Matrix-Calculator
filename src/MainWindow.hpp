// MainWindow.hpp — Main window for Matrix Calculator: matrix inputs, result view, operation controls.

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "matrix.hpp"
#include <QMainWindow>
#include <QTableWidget>

class QLineEdit;
class QSpinBox;
class QGroupBox;
class QLabel;
class QPushButton;
class QStatusBar;
class QScrollArea;
class QWidget;
class QGridLayout;
class QVBoxLayout;
class QHBoxLayout;

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;

private slots:
  void onRowsColsChanged();
  void performAddition();
  void performSubtractionAB();
  void performSubtractionBA();
  void performMultiplyAB();
  void performMultiplyBA();
  void performScalarMultiplyA();
  void performScalarMultiplyB();
  void performScalarDivideA();
  void performScalarDivideB();
  void performRREFOnA();
  void performRREFOnB();
  void performInverseA();
  void performInverseB();

private:
  void setupUi();
  void buildMatrixInputs();
  void buildOperationPanel();
  void buildResultView();
  void runInternalTests();

  Matrix loadMatrixFromTable(QTableWidget* table) const;
  void displayMatrixInTable(const Matrix& M, QTableWidget* table);
  void setResult(const Matrix& M);
  void showError(const QString& message);
  void showStatus(const QString& message);

  QSpinBox* rowsA_;
  QSpinBox* colsA_;
  QTableWidget* tableA_;
  QSpinBox* rowsB_;
  QSpinBox* colsB_;
  QTableWidget* tableB_;
  QTableWidget* resultTable_;
  QLineEdit* scalarEdit_;
  QStatusBar* statusBar_;
  QWidget* centralWidget_;
};

#endif // MAINWINDOW_HPP
