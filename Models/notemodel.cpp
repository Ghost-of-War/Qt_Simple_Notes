#include "notemodel.h"

NoteModel::NoteModel(QObject *parent) : QAbstractListModel(parent) {}

const QList<Note>& NoteController::notes() const
{
    return m_notes;
}

int NoteModel::rowCount(const QModelIndex &parent) const{
    if (parent.isValid()||!m_controller){
        return 0;
    }
    return m_controller->notes().size();
}

QVariant NoteModel::data(const QModelIndex &index, int role) const{
    if(!index.isValid()||!m_controller){
        return QVariant();
    }

    const Note &note = m_controller->notes().at(index.row());

    switch (role) {
        case IdRole: return note.id;
        case TitleRole: return note.title;
        case TextRole: return note.text;
        case PriorityRole: return static_cast<int>(note.priority);
        case TimestampRole: return note.timestamp.toString("dd.MM.yyyy HH:mm");

        default: return QVariant();
    }
}

QHash<int, QByteArray> NoteModel::roleNames() const{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[TitleRole] = "title";
    roles[TextRole] = "text";
    roles[PriorityRole] = "priority";
    roles[TimestampRole] = "timestamp";
    return roles;
}

NoteController* NoteModel::controller() const
{
    return m_controller;
}

void NoteModel::setController(NoteController *controller)
{
    if (m_controller == controller) return;
    beginResetModel();

    if (m_controller) {
        m_controller->disconnect(this);
    }

    m_controller = controller;

    if (m_controller) {
        //add new note
        connect(m_controller, &NoteController::noteAdded, this, [this]() {
            int row = m_controller->notes().size() - 1;
            beginInsertRows(QModelIndex(), row, row);
            endInsertRows();
        });

        //begin of deleting
        connect(m_controller, &NoteController::preItemRemoved, this, [this](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });

        //end the deleting
        connect(m_controller, &NoteController::postItemRemoved, this, [this]() {
            endRemoveRows();
        });
        //begin of sorting
        connect(m_controller, &NoteController::preNotesReordered, this, [this]() {
            emit layoutAboutToBeChanged();
        });

        //end of sorting
        connect(m_controller, &NoteController::postNotesReordered, this, [this]() {
            emit layoutChanged();
        });

        //text editing
        connect(m_controller, &NoteController::noteChanged, this, [this](int index) {
            QModelIndex modelIndex = this->index(index, 0);
            emit dataChanged(modelIndex, modelIndex);
        });
    }

    endResetModel();
    emit controllerChanged();
}
