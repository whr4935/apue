#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

class SortDialog : public QDialog
{
	Q_OBJECT

public:
	SortDialog(QWidget *parent = 0);
	~SortDialog();

	void setColumnRange(QChar first, QChar last);

private slots:
	void updateMoreText(bool);	

public:
	QPushButton *okButton;
	QPushButton *cancelButton;
	QPushButton *moreButton;

	QGroupBox *firstGroupBox;
	QLabel *firstColumnLabel;
	QLabel *firstOrderLabel;
	QComboBox *firstColumnComboBox;
	QComboBox *firstOrderComboBox;
	
	QGroupBox *secondGroupBox;
	QLabel *secondColumnLabel;
	QLabel *secondOrderLabel;
	QComboBox *secondColumnComboBox;
	QComboBox *secondOrderComboBox;

	QGroupBox *thirdGroupBox;
	QLabel *thirdColumnLabel;
	QLabel *thirdOrderLabel;
	QComboBox *thirdColumnComboBox;
	QComboBox *thirdOrderComboBox;
};

#endif // SORTDIALOG_H
