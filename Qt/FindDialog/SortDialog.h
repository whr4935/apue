#ifndef SORTDIALOG_H
#define SORTDIALOG_H

#include <QDialog>

namespace Ui {
    class SortDialog;
}

class SortDialog : public QDialog
{
    Q_OBJECT

public:
    SortDialog(QWidget* parent = 0);
    void setColumnRange(QChar first, QChar last);
    ~SortDialog();

private slots:
    void setAdvancedButtonText(bool toggled);

private:
    Ui::SortDialog *ui;
};

#endif // SORTDIALOG_H
