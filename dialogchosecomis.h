#ifndef DIALOGCHOSECOMIS_H
#define DIALOGCHOSECOMIS_H

#include <QDialog>

namespace Ui {
class DialogChoseComis;
}

class DialogChoseComis : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChoseComis(const QStringList& years, const QStringList& teachers, QWidget* parent = nullptr);
    ~DialogChoseComis();

private slots:
    void on_pb_comis_clicked();

signals:
    void sendDataChoseComis(const QString& id_year, const QString& id_ruk, const QString& podl);

private:
    Ui::DialogChoseComis* ui;
};

#endif // DIALOGCHOSECOMIS_H
