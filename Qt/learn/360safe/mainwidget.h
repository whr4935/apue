#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "dropshadowwidget.h"
#include <QWidget>

class MainWidget : public DropShadowWidget
{
	Q_OBJECT

public:
	MainWidget(QWidget *parent = 0);
	~MainWidget();

private:
	
};

#endif // MAINWIDGET_H
