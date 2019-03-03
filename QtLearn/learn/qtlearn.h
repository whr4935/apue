#ifndef QTLEARN_H
#define QTLEARN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSplitter>
#include <QScrollArea>
#include <QTextEdit>


class MainWidget;

class QtLearn : public QWidget
{
	Q_OBJECT

public:
	QtLearn(QWidget *parent = 0);
	~QtLearn();

signals:
	void sendQuit(int arg);

private slots:
	void onClicked();
	void onQuit(int arg);

private:
	QPushButton *m_btn;
	QPushButton *m_btn2;
	QLabel *m_label;
	QSplitter *m_splitter1;
	MainWidget *m_mainWidget;
	QScrollArea *m_scrollArea;
	QTextEdit *m_textEdit;
};

#endif // QTLEARN_H
