#include "titlewidget.h"
#include <QHBoxLayout>

TitleWidget::TitleWidget(QWidget *parent)
	: QWidget(parent)
{
	version_title = new QLabel();
	skin_button = new PushButton();
	main_menu_button = new PushButton();
	min_button = new PushButton();
	feedback_button = new PushButton();
	close_button = new PushButton();
	medal_button = new PushButton();

	version_title->setObjectName("whiteLabel");

	medal_button->setPicName(QString(":/sysButton/medal"));
	skin_button->setPicName(QString(":/sysButton/skin"));
	min_button->setPicName(QString(":/sysButton/min"));
	close_button->setPicName(QString(":/sysButton/close"));
	main_menu_button->setPicName(QString(":/sysButton/menu"));
	feedback_button->setPicName(QString(":/sysButton/feedback"));

	connect(skin_button, SIGNAL(clicked()), this, SIGNAL(showSkin()));
	connect(main_menu_button, SIGNAL(clicked()), this, SIGNAL(showMainMenu()));
	connect(min_button, SIGNAL(clicked()), this, SIGNAL(showMin()));
	connect(close_button, SIGNAL(clicked()), this, SIGNAL(closeWidget()));

	QHBoxLayout  *title_layout = new QHBoxLayout();
	title_layout->addWidget(version_title, 0, Qt::AlignCenter);
}

TitleWidget::~TitleWidget()
{

}

void TitleWidget::turnPage(QString current_page)
{

}
