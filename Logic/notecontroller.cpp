#include "notecontroller.h"
#include <algorithm>
#include <QDebug>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>

NoteController::NoteController(QObject *parent) : QObject(parent), m_nextId(0) {
    loadNotesFromJson();
}

void NoteController::addNote(const QString &title, const QString &text, Priority priority){
    Note note;

    note.id = m_nextId++;
    note.title = title;
    note.text = text;
    note.priority = priority;
    note.timestamp = QDateTime::currentDateTime();

    m_notes.append(note);

    qDebug() << "New note added. ID:" << note.id << "Priority:" << static_cast<int>(priority);

    emit noteAdded();
    saveNotesToJson();
}

void NoteController::editNote(int id, const QString &newTitle, const QString &newText, int newPriority) {
    for (int i = 0; i < m_notes.size(); ++i) {
        if (m_notes[i].id == id) {

            Priority parsedPriority = static_cast<Priority>(newPriority);

            if (newPriority < 0 || newPriority > static_cast<int>(Priority::Urgent)) {
                qDebug() << "Invalid priority value:" << newPriority;
                return;
            }

            if (m_notes[i].title != newTitle ||
                m_notes[i].text != newText ||
                m_notes[i].priority != parsedPriority) {

                m_notes[i].title = newTitle;
                m_notes[i].text = newText;
                m_notes[i].priority = parsedPriority;

                m_notes[i].timestamp = QDateTime::currentDateTime();

                emit noteChanged(i);
                saveNotesToJson();
            }
            return;
        }
    }
}
void NoteController::deleteNote(int id){
    for(int i = 0; i < m_notes.size(); ++i) {
        if(m_notes[i].id == id) {
            emit preItemRemoved(i);

            m_notes.removeAt(i);

            emit postItemRemoved();
            saveNotesToJson();

            qDebug() << "Note deleted with ID:" << id;
            return;
        }
    }
}

void NoteController::updateNotePriority(int id, int newPriority) {
    for (int i = 0; i < m_notes.size(); ++i) {
        if (m_notes[i].id == id) {
            m_notes[i].priority = static_cast<Priority>(newPriority);
            emit noteChanged(i);
            return;
        }
    }
}

// Higher enum value = higher priority
void NoteController::sortPriority(){
    emit preNotesReordered();

    std::sort(m_notes.begin(), m_notes.end(), [](const Note &a, const Note &b){
        if(a.priority!=b.priority){
            return static_cast<int>(a.priority)>static_cast<int>(b.priority);
        }
        return a.timestamp>b.timestamp;
    });

    emit postNotesReordered();
}

void NoteController::sortDate(){
    emit preNotesReordered();

    std::sort(m_notes.begin(), m_notes.end(), [](const Note &a, const Note &b){
        if(a.timestamp!=b.timestamp){
            return a.timestamp>b.timestamp;
        }
        return a.priority<b.priority;
    });

    emit postNotesReordered();
}

void NoteController::saveNotesToJson() {
    QJsonArray jsonArray;

    for (const Note& note : m_notes) {
        QJsonObject jsonObj;
        jsonObj["id"] = note.id;
        jsonObj["title"] = note.title;
        jsonObj["text"] = note.text;
        jsonObj["priority"] = static_cast<int>(note.priority);

        jsonObj["timestamp"] = note.timestamp.toString(Qt::ISODate);

        jsonArray.append(jsonObj);
    }

    QJsonDocument doc(jsonArray);

    QFile file("notes_data.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "Notes successfully saved to notes_data.json";
    } else {
        qDebug() << "Error: Could not open file for writing.";
    }
}

void NoteController::loadNotesFromJson() {
    QFile file("notes_data.json");

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        qDebug() << "Save file not found. Starting with a clean slate.";
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;

    QJsonArray jsonArray = doc.array();

    m_notes.clear();

    int maxId = -1;

    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject jsonObj = jsonArray[i].toObject();
        Note note;

        note.id = jsonObj["id"].toInt();
        note.title = jsonObj["title"].toString();
        note.text = jsonObj["text"].toString();
        note.priority = static_cast<Priority>(jsonObj["priority"].toInt());

        note.timestamp = QDateTime::fromString(jsonObj["timestamp"].toString(), Qt::ISODate);

        m_notes.append(note);

        if (note.id > maxId) {
            maxId = note.id;
        }
    }

    m_nextId = maxId + 1;

    qDebug() << "Loaded notes:" << m_notes.size() << ". Next ID:" << m_nextId;
}
