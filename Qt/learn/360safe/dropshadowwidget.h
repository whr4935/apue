#ifndef DROPSHADOWWIDGET_H
#define DROPSHADOWWIDGET_H

#include <QDialog>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <qmath.h>

class DropShadowWidget : public QDialog
{
	Q_OBJECT

public:
	DropShadowWidget(QWidget *parent = 0);
	~DropShadowWidget();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent *event);

private:
	QPoint move_point;
	bool mouse_press;
};

#endif // DROPSHADOWWIDGET_H
