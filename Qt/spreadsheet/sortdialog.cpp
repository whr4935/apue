#include "sortdialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>


SortDialog::SortDialog(QWidget *parent)
	: QDialog(parent)
{
	firstColumnLabel = new QLabel("Column");
	firstColumnComboBox = new QComboBox;
	firstOrderLabel = new QLabel("Order");
	firstOrderComboBox = new QComboBox;
	firstOrderComboBox->addItem("Ascending");
	firstOrderComboBox->addItem("Descending");
	firstGroupBox = new QGroupBox("&Primary Key");

	QGridLayout *gridLayout = new QGridLayout;
	gridLayout->addWidget(firstColumnLabel, 0, 0);
	gridLayout->addWidget(firstColumnComboBox, 0, 1);
	gridLayout->setRowStretch(0, 0);
	gridLayout->addWidget(firstOrderLabel, 1, 0);
	gridLayout->addWidget(firstOrderComboBox, 1, 1, 1, 2);
	firstGroupBox->setLayout(gridLayout);

	secondColumnLabel = new QLabel("Column");
	secondColumnComboBox = new QComboBox;
	secondOrderLabel = new QLabel("Order");
	secondOrderComboBox = new QComboBox;
	secondOrderComboBox->addItem("Ascending");
	secondOrderComboBox->addItem("Descending");
	secondGroupBox = new QGroupBox("&Secondary Key");

	QGridLayout *gridLayout2 = new QGridLayout;
	gridLayout2->addWidget(secondColumnLabel, 0, 0);
	gridLayout2->addWidget(secondColumnComboBox, 0, 1);
	gridLayout2->setRowStretch(0, 0);
	gridLayout2->addWidget(secondOrderLabel, 1, 0);
	gridLayout2->addWidget(secondOrderComboBox, 1, 1, 1, 2);
	secondGroupBox->setLayout(gridLayout2);

	thirdColumnLabel = new QLabel("Column");
	thirdColumnComboBox = new QComboBox;
	thirdOrderLabel = new QLabel("Order");
	thirdOrderComboBox = new QComboBox;
	thirdOrderComboBox->addItem("Ascending");
	thirdOrderComboBox->addItem("Descending");
	thirdGroupBox = new QGroupBox("&Tertiary Key");

	QGridLayout *gridLayout3 = new QGridLayout;
	gridLayout3->addWidget(thirdColumnLabel, 0, 0);
	gridLayout3->addWidget(thirdColumnComboBox, 0, 1);
	gridLayout3->setRowStretch(0, 0);
	gridLayout3->addWidget(thirdOrderLabel, 1, 0);
	gridLayout3->addWidget(thirdOrderComboBox, 1, 1, 1, 2);
	thirdGroupBox->setLayout(gridLayout3);

	okButton = new QPushButton("OK");
	cancelButton = new QPushButton("Cancel");
	moreButton = new QPushButton("More");
	moreButton->setCheckable(true);

	QVBoxLayout *groupLayout = new QVBoxLayout;
	groupLayout->addWidget(firstGroupBox);
	groupLayout->addStretch();
	groupLayout->addWidget(secondGroupBox);
	groupLayout->addWidget(thirdGroupBox);

	QVBoxLayout *btnLayout = new QVBoxLayout;
	btnLayout->addWidget(okButton);
	btnLayout->addWidget(cancelButton);
	btnLayout->addStretch();
	btnLayout->addWidget(moreButton);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addLayout(groupLayout);
	mainLayout->addLayout(btnLayout);
	setLayout(mainLayout);

	secondGroupBox->hide();
	thirdGroupBox->hide();
	layout()->setSizeConstraint(QLayout::SetFixedSize);
	setColumnRange('A', 'Z');
	setWindowTitle("SortDialog");

	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(moreButton, SIGNAL(toggled(bool)), secondGroupBox, SLOT(setVisible(bool)));
	connect(moreButton, SIGNAL(toggled(bool)), thirdGroupBox, SLOT(setVisible(bool)));
	connect(moreButton, SIGNAL(toggled(bool)), this, SLOT(updateMoreText(bool)));
}

SortDialog::~SortDialog()
{

}

void SortDialog::setColumnRange(QChar first, QChar last)
{
	firstColumnComboBox->clear();
	secondColumnComboBox->clear();
	thirdColumnComboBox->clear();

	secondColumnComboBox->addItem("None");
	thirdColumnComboBox->addItem("None");
	firstColumnComboBox->setMinimumSize(secondColumnComboBox->sizeHint());

	QChar ch = first;
	while (ch <= last) {
		firstColumnComboBox->addItem(QString(ch));
		secondColumnComboBox->addItem(QString(ch));
		thirdColumnComboBox->addItem(QString(ch));
		ch = ch.unicode() + 1;
	}
}

void SortDialog::updateMoreText(bool b)
{
	QString text;
	if (b) {
		text = "Advanced <<";
	} else {
		text = "Advanced >>";
	}
	moreButton->setText(text);
}
