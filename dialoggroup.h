#ifndef DIALOGGROUP_H
#define DIALOGGROUP_H

#include <QDialog>

namespace Ui {
class DialogGroup;
}

class DialogGroup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGroup(QWidget* parent = nullptr);
    ~DialogGroup();

private slots:
    void on_pb_group_clicked();

signals:
    void sendDataGroup(const QString& year, const QString& group_name);

private:
    Ui::DialogGroup* ui;
};

#endif // DIALOGGROUP_H
