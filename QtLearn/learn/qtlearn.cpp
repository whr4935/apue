#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QSizePolicy>
#include <QLibraryInfo>
#include <QCoreApplication>
#include <QtWidgets>

#include "qtlearn.h"
#include "360safe/mainwidget.h"

extern QWidget* plotter_main();

QtLearn::QtLearn(QWidget *parent)
    : QWidget(parent), m_mainWidget(0)
{
// 	QPalette palette = this->palette();
// 	palette.setBrush(QPalette::Window, Qt::cyan);
// 	setPalette(palette);
//	setAutoFillBackground(true);

    qDebug() << this->size();
// 	qDebug() << sizeHint();
// 	qDebug() << this->minimumSizeHint();
// 	qDebug() << this->sizePolicy();
//	qDebug() << this->contentsMargins();

    m_btn = new QPushButton("Button1");
// 	qDebug() << "button:";
// 	qDebug() << m_btn->size();
// 	qDebug() << m_btn->sizeHint();
// 	qDebug() << m_btn->minimumSizeHint();
// 	qDebug() << m_btn->sizePolicy();
//	qDebug() << m_btn->contentsMargins();

    QSizePolicy size_policy;
    size_policy.setHorizontalStretch(0);
    size_policy.setHorizontalPolicy(QSizePolicy::Expanding);
    size_policy.setVerticalPolicy(QSizePolicy::Fixed);
    m_btn->setSizePolicy(size_policy);

    m_btn2 = new QPushButton("Button2");
    size_policy.setHorizontalStretch(0);
    size_policy.setHorizontalPolicy(QSizePolicy::Preferred);
    size_policy.setVerticalPolicy(QSizePolicy::Fixed);
    m_btn2->setSizePolicy(size_policy);

    m_label = new QLabel("Hello");
//	qDebug() << m_label->contentsMargins();

    QHBoxLayout *h_layout = new QHBoxLayout();
// 	qDebug() << h_layout->spacing();
// 	qDebug() << h_layout->contentsMargins();

    h_layout->addWidget(m_btn, 0);
    h_layout->addWidget(m_btn2, 0);
//    h_layout->addStretch(0);
    h_layout->addWidget(m_label, 0);
//	qDebug() << m_btn->sizePolicy().horizontalStretch();
//	h_layout->addStretch(0);
//	h_layout->addWidget(m_btn2);
//	h_layout->addStretch(0);
//	h_layout->addWidget(m_label);
//	m_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


//	m_scrollArea = new QScrollArea;
//	m_textEdit = new QTextEdit;
//	m_scrollArea->setWidget(m_textEdit);
//	m_scrollArea->setWidgetResizable(true);

//	m_splitter1 = new QSplitter(Qt::Horizontal);
//	m_splitter1->addWidget(m_btn2);
//	m_splitter1->addWidget(m_label);
//	m_splitter1->addWidget(m_scrollArea);
//    m_splitter1->setStretchFactor(2, 0);

// 	qDebug() << m_splitter1->contentsMargins();
// 	qDebug() << m_splitter1->handleWidth();
//	m_splitter1->setContentsMargins(10, 10, 10, 10);
//	h_layout->addWidget(m_splitter1);

    h_layout->setSizeConstraint(QLayout::SetMaximumSize);
    setLayout(h_layout);
    qDebug() << this->size();
    qDebug() << sizeHint();
    qDebug() << this->minimumSizeHint();
    qDebug() << this->sizePolicy();



    //resize(600, 500);

    connect(m_btn, SIGNAL(clicked()), this, SLOT(onClicked()));
    connect(this, SIGNAL(sendQuit(int)), this, SLOT(onQuit(int)));
}

QtLearn::~QtLearn()
{
    if (m_mainWidget)
        m_mainWidget->deleteLater();
}

void QtLearn::onClicked()
{
    static int cnt;

    emit sendQuit(++cnt);
}

void QtLearn::onQuit(int arg)
{
    static QWidget *w = NULL;

//	if (w == NULL) {
//		//w = new GoToCellDialog;
//		//w = new SortDialog;
//		w = plotter_main();
//	}
//	if (w)
//		w->show();



    //qDebug() << "hello world " << arg;
    if (m_mainWidget == NULL) {
        //m_mainWidget = new MainWidget();

        m_mainWidget->show();
    }
}
