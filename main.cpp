#include "Logic/notecontroller.h"
#include "Models/notemodel.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    NoteController controller;

    qmlRegisterType<NoteModel>("NotesApp", 1, 0, "NoteModel");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("noteController", &controller);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Qt_Simple_Notes", "Main");

    return app.exec();
}
