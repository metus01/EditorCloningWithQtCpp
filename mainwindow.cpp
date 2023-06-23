#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QFrame>
#include <QLabel>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QList>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QLabel *mylabel = new QLabel(this);
    //mylabel->setText("Hello Wordl");
    //setCentralWidget(mylabel);
    /*QPushButton *mybutton = new QPushButton(this);
    mybutton->setText("Quit");
    connect(mybutton , SIGNAL(clicked(bool)) , this , SLOT(quitClicked()));*/
    /*QTabWidget *tabsWidgets = new QTabWidget(this);
    tabsWidgets->addTab(new QWidget , "Tab 1");
    tabsWidgets->addTab(new QWidget , "Tab 2");
    tabsWidgets->addTab(new QWidget , "Tab 3");*/
    tabsWidget->setMovable(true);
    tabsWidget->setTabsClosable(true);
    setCentralWidget(window);
    window->addWidget(treeView);
    window->addWidget(tabsWidget);
    treeView->setMaximumWidth(0);
    treeView->setMinimumWidth(0);
    //tabsWidgets->show();
    connect(treeView , SIGNAL(clicked(QModelIndex)) , this ,SLOT(openTreeViewFile(QModelIndex)) );
    connect(tabsWidget , SIGNAL(tabCloseRequested(int)) , this , SLOT(closeTab(int)) );
    QFontDatabase::addApplicationFont(":/Cascadia Code Bold 700.otf");

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::quitClicked()
{
    QApplication::quit();
}


void MainWindow::on_actionNew_File_triggered()
{
    MainWindow::createTab();
}

void MainWindow::on_actionClose_File_triggered()
{
    tabsWidget->removeTab(tabsWidget->currentIndex());
}
void MainWindow::closeTab(int index)
{
    tabsWidget->removeTab(index);
}
void MainWindow::createTab()
{
    QFrame *tabFrame = new QFrame(this);
    QVBoxLayout *tabLayout = new QVBoxLayout(tabFrame);
    QPlainTextEdit *fileEdit =    new QPlainTextEdit();
    QFont font = fileEdit->document()->defaultFont();
    font.setFamily("Cascadia Code Light");
    fileEdit->setFont(font);
    fileEdit->setObjectName("textEdit");
    // fileEdit->setTabStopWidth(QFontMetrics(fileEdit->font()).horizontalAdvance('') * 4);
    //fileEdit->setTabStopDistance(QFontMetrics(fileEdit->font()).horizontalAdvance('')*4);
    QLabel *status = new QLabel(this);
    status->setText("Line 1 ,  Column 1");
    status ->setObjectName("status");
    tabLayout->addWidget(fileEdit);
    tabLayout->addWidget(status);
    int tab= tabsWidget->addTab(tabFrame , "Utitled");
    tabsWidget->setCurrentIndex(tab);
    tabsWidget->setTabToolTip(tabsWidget->currentIndex(),"Untitled");
   connect(MainWindow::currentTextEdit() , SIGNAL(cursorPositionChanged()) , this ,  SLOT(updateStatus()));
    connect(MainWindow::currentTextEdit() , SIGNAL(textChanged()) , this ,  SLOT(textEditChanged()));
}


void MainWindow::on_actionOpen_File_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this , "Open the File");
    MainWindow::createTab();
    MainWindow::openTabFile(filePath);
}
void MainWindow::openTabFile(QString filePath)
{
    QFile file(filePath);
    QFileInfo fileName(filePath);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this , "Warning "  , "Cannot open file" +file.errorString());
        return;
    }
    tabsWidget->setTabToolTip(tabsWidget->currentIndex(),filePath);
    QTextStream in(&file);
    QString  text = in.readAll();
    MainWindow::currentTextEdit()->setPlainText(text);
    file.close();
    tabsWidget->setTabText(tabsWidget->currentIndex() , fileName.fileName() );
}
QPlainTextEdit* MainWindow::currentTextEdit()
{
    QList <QPlainTextEdit *> fileEditList = tabsWidget->findChildren<QPlainTextEdit *>("textEdit");
    // return fileEditList[tabsWidget->currentIndex()];
    for(int i=0 ; i < fileEditList.count() ; i++)
    {
        if(tabsWidget->indexOf(fileEditList[i]->parentWidget()) == tabsWidget->currentIndex())
         return fileEditList[i];

    }
    return new QPlainTextEdit;
}
 void MainWindow::textEditChanged()
 {
    QString tabName = tabsWidget->tabText(tabsWidget->currentIndex());
    if(tabName.at(0) != '*')
    {
        tabsWidget->setTabText(tabsWidget->currentIndex()  , "*" +tabName);
    }
 }


void MainWindow::on_actionSave_triggered()
{
    QString fileName = tabsWidget->tabToolTip(tabsWidget->currentIndex());
    if(fileName == "Untitled")
    {
        MainWindow::on_actionSave_as_triggered();
        return;
    }
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly) || QFile::Text)
    {
        QMessageBox::warning(this, "warning" , "cannot save file" , file.errorString());
    }
    QTextStream out(&file);

        QString text = MainWindow::currentTextEdit()->toPlainText();
        out << text;
        file.close();
        QString newTabText = tabsWidget->tabText(tabsWidget->currentIndex()).remove(0,1);
        tabsWidget->setTabText(tabsWidget->currentIndex() , newTabText);

}

void MainWindow::on_actionSave_as_triggered()
{
    if(tabsWidget->count() == 0)
    {
       QMessageBox::warning(this , "warning" , "Cannot save file!");
       return;
    }
    QString filePath = QFileDialog::getSaveFileName(this , "Save As..");
    QFile file(filePath);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this , "warning" , "Cannot save file" + file.errorString());
        return;
    }
    QTextStream out(&file);
    QString text  = MainWindow::currentTextEdit()->toPlainText();
    out << text;
    file.close();
    MainWindow::openTabFile(filePath);


}
void MainWindow::updateStatus()
{
    QString line  =QString::number(MainWindow::currentTextEdit()->textCursor().blockNumber()+1);
    QString column = QString::number(MainWindow::currentTextEdit()->textCursor().columnNumber()+1);
    QString newStatus = "Line    " +  line  +  "Column  " + column;
    MainWindow::currentStatus()->setText(newStatus);
}
QLabel* MainWindow::currentStatus()
{
    QList <QLabel*> statusList = tabsWidget->findChildren<QLabel *>("status");
    // return fileEditList[tabsWidget->currentIndex()];
    for(int i=0 ; i < statusList.count() ; i++)
    {
        if(tabsWidget->indexOf(statusList[i]->parentWidget()) == tabsWidget->currentIndex())
         return statusList[i];

    }
    return new QLabel;
}

void MainWindow::on_actionOpen_Folder_triggered()
{
    QUrl direPath = QFileDialog::getExistingDirectory(this , "Open Folder" , "/" , QFileDialog::ShowDirsOnly);
    dirModel->setRootPath(direPath.toString());
    treeView->setModel(dirModel);
    //treeView->setRootIndex(dirModel);
    treeView->setRootIndex(dirModel->index(direPath.toString()));
    treeView->setMinimumWidth(width()*20/100);
    treeView->setMaximumWidth(width() * 30/100);
    treeView->hideColumn(1);
     treeView->hideColumn(2);
      treeView->hideColumn(3);

}
void MainWindow::openTreeViewFile(QModelIndex index)
{
    MainWindow::createTab();
    QString filePath = dirModel->fileInfo(index).absoluteFilePath();
    MainWindow::openTabFile(filePath);
}
