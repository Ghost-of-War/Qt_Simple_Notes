#ifndef NOTEMODEL_H
#define NOTEMODEL_H

#include <QAbstractListModel>
#include <Logic/notecontroller.h>

class NoteModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(NoteController* controller READ controller WRITE setController NOTIFY controllerChanged)
public:
    enum NoteRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        TextRole,
        PriorityRole,
        TimestampRole
    };

    explicit NoteModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    NoteController* controller() const;
    void setController(NoteController *controller);

private:
    NoteController *m_controller = nullptr;

signals:
    void controllerChanged();
};

#endif // NOTEMODEL_H
