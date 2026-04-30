#ifndef NOTECONTROLLER_H
#define NOTECONTROLLER_H

#include <QObject>
#include <QQmlEngine>
#include <QList>
#include <Data/Note.h>

class NoteController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit NoteController(QObject *parent = nullptr);

    const QList<Note>& notes() const;

    Q_INVOKABLE void addNote(const QString &title, const QString &text, Priority priority = Priority::JustInfo);
    Q_INVOKABLE void editNote(int id, const QString &newTitle, const QString &newText, int newPriority);
    Q_INVOKABLE void deleteNote(int id);

    Q_INVOKABLE void updateNotePriority(int id, int newPriority);

    Q_INVOKABLE void sortDate();
    Q_INVOKABLE void sortPriority();

    Q_INVOKABLE void saveNotesToJson();
    Q_INVOKABLE void loadNotesFromJson();



private:
    QList<Note> m_notes;
    int m_nextId = 0;

signals:
    void noteAdded();
    void noteChanged(int index);

    void preItemRemoved(int index);
    void postItemRemoved();

    void preNotesReordered();
    void postNotesReordered();
};

#endif // NOTECONTROLLER_H
