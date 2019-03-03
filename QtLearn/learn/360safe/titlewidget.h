#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include "push_button.h"
#include "tool_button.h"

class TitleWidget : public QWidget
{
	Q_OBJECT

public:
	TitleWidget(QWidget *parent = 0);
	~TitleWidget();

signals:
	void showSkin();
	void showMin();
	void showMainMenu();
	void closeWidget();
	void turnPage(int current_page);

private slots:
	void turnPage(QString current_page);

private:
	QPoint press_point;
	bool is_move;
	QLabel *version_title;
	PushButton *medal_button;
	PushButton *skin_button;
	PushButton *main_menu_button;
	PushButton *min_button;
	PushButton *feedback_button;
	PushButton *close_button;

	QLabel *safe_360_label;
	QLabel *safe_label;
	QList<ToolButton*> button_list;
};

#endif // TITLEWIDGET_H
