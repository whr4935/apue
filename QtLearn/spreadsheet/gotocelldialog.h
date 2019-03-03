#ifndef GOTOCELLDIALOG_H
#define GOTOCELLDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QLabel>

class GoToCellDialog : public QDialog
{
	Q_OBJECT

public:
	GoToCellDialog(QWidget *parent = 0);
	~GoToCellDialog();

private slots:
	void textChanged();

public:
	QLabel *label;
	QLineEdit *lineEdit;
	QDialogButtonBox *button;
};

#endif // GOTOCELLDIALOG_H
