#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class GoToCellDialog ;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();

private slots:
    void onTextChanged();

private:
    Ui::GoToCellDialog *ui;
};

#endif // FINDDIALOG_H
