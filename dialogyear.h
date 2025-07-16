#ifndef DIALOGYEAR_H
#define DIALOGYEAR_H

#include <QDialog>
#include <QDebug>

namespace Ui {
class DialogYear;
}

class DialogYear : public QDialog
{
    Q_OBJECT

public:
    explicit DialogYear(QWidget* parent = nullptr);
    ~DialogYear();

private slots:
    void on_pb_year_clicked();

signals:
    void sendDataYear(const QString& year, const QString& years);

private:
    Ui::DialogYear* ui;
};

#endif // DIALOGYEAR_H
