#include "finddialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

FindDialog::FindDialog(QWidget *parent)
	: QDialog(parent)
{
	label = new QLabel("Find &what");
	lineEdit = new QLineEdit();
	label->setBuddy(lineEdit);
	caseCheckBox = new QCheckBox("Match &case");
	backwardCheckBox = new QCheckBox("Search &backward");
	findButton = new QPushButton("&Find");
	findButton->setDefault(true);
	findButton->setEnabled(false);
	closeButton = new QPushButton("Close");

	QHBoxLayout *h_layout =  new QHBoxLayout;
	h_layout->addWidget(label);
	h_layout->addWidget(lineEdit);
	
	QVBoxLayout *v_layout = new QVBoxLayout;
	v_layout->addWidget(findButton);
	v_layout->addWidget(closeButton);
	v_layout->addStretch();

	QVBoxLayout *v__checkbox_layout = new QVBoxLayout;
	v__checkbox_layout->addLayout(h_layout);
	v__checkbox_layout->addWidget(caseCheckBox);
	v__checkbox_layout->addWidget(backwardCheckBox);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addLayout(v__checkbox_layout);
	layout->addLayout(v_layout);

	setLayout(layout);

	setTabOrder(findButton, closeButton);
	setTabOrder(closeButton, label);
	setTabOrder(label, backwardCheckBox);
	setTabOrder(backwardCheckBox, caseCheckBox);
	
	setWindowTitle(tr("Find"));
	setFixedHeight(sizeHint().height());

	connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableFindButton(const QString&)));
	connect(findButton, SIGNAL(clicked()), this, SLOT(findClicked()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

FindDialog::~FindDialog()
{

}

void FindDialog::findClicked()
{
	QString text = lineEdit->text();
	Qt::CaseSensitivity cs = caseCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
	if (backwardCheckBox->isChecked()) {
		emit findPrevious(text, cs);
	} else {
		emit findNext(text, cs);
	}
}

void FindDialog::enableFindButton(const QString &text)
{
	findButton->setEnabled(!text.isEmpty());
}
