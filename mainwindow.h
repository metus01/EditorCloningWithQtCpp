#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QLabel>
#include <QTreeView>
#include <QSplitter>
#include <QFileSystemModel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void quitClicked();

    void on_actionNew_File_triggered();

    void on_actionClose_File_triggered();
    void closeTab( int index);
    void createTab();

    void on_actionOpen_File_triggered();
    void openTabFile(QString filePath);
    QPlainTextEdit*  currentTextEdit();
    void  textEditChanged();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();
    void updateStatus();
    QLabel* currentStatus();

    void on_actionOpen_Folder_triggered();
    void openTreeViewFile(QModelIndex index);
private:
    Ui::MainWindow *ui;
    QTabWidget *tabsWidget = new QTabWidget(this);
    QTreeView *treeView = new QTreeView;
    QSplitter *window = new QSplitter;
    QFileSystemModel *dirModel  = new QFileSystemModel(this);

};

#endif // MAINWINDOW_H
