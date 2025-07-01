#ifndef DIALOGSEARCH_H
#define DIALOGSEARCH_H

#include <QDialog>

namespace Ui {
class DialogSearch;
}

class DialogSearch : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSearch(QWidget *parent = nullptr);
    ~DialogSearch();

private slots:
    void on_pb_search_clicked();

signals:
    void sendDataSearch(const QString& search);

private:
    Ui::DialogSearch *ui;
};

#endif // DIALOGSEARCH_H
