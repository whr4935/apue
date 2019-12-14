#include "gotocelldialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QLineEdit>

GoToCellDialog::GoToCellDialog(QWidget *parent)
	: QDialog(parent)
{
	label = new QLabel("Cell Location:");
	lineEdit = new QLineEdit();
	label->setBuddy(lineEdit);
	QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
	lineEdit->setValidator(new QRegExpValidator(regExp, this));

	button = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
	button->button(QDialogButtonBox::Ok)->setEnabled(false);

	QHBoxLayout *upLayout = new QHBoxLayout;
	upLayout->addWidget(label);
	upLayout->addWidget(lineEdit);

	QHBoxLayout *downLayout = new QHBoxLayout;
	downLayout->addStretch();
	downLayout->addWidget(button);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addLayout(upLayout);
	layout->addLayout(downLayout);

	setLayout(layout);

	setFixedHeight(sizeHint().height());

	connect(button, SIGNAL(accepted()), this, SLOT(accept()));
	connect(button, SIGNAL(rejected()), this, SLOT(reject()));
	connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(textChanged()));
}

GoToCellDialog::~GoToCellDialog()
{

}

void GoToCellDialog::textChanged()
{
	button->button(QDialogButtonBox::Ok)->setEnabled(lineEdit->hasAcceptableInput());
}
