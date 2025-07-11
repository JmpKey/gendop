#ifndef DIALOGCHOSEGROUP_H
#define DIALOGCHOSEGROUP_H

#include <QDialog>

namespace Ui {
class DialogChoseGroup;
}

class DialogChoseGroup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChoseGroup(const QStringList& groups, QWidget* parent = nullptr);
    ~DialogChoseGroup();

private slots:
    void on_pb_c_group_clicked();

signals:
    void sendDataChoseGroup(const QString& id_c_group);

private:
    Ui::DialogChoseGroup* ui;
};

#endif // DIALOGCHOSEGROUP_H
