#ifndef GREENHOUSELOG_H
#define GREENHOUSELOG_H

#include <QString>
#include <QFile>
#include <QDate>
#include <QTimer>

class GreenHouseLog : public QFile
{
public:
    GreenHouseLog();
    ~GreenHouseLog();

    void set_filename(QString name);
    void set_dirname(QString dir);
    QString get_filename();
    QString get_dirname();
    bool start_logging();
    bool stop_logging();
    void set_logging_state(bool);
    bool get_logging_state(void);
    bool wrire_to_file(QByteArray &str);

private:
    QString filename;
    QString dirname;
    QFile file;
    QTimer timer;
    bool logging = false;
    bool create_file();
    void update_filename();

signals:
    void counterChanged(int counter);
private slots:
    void realTimeout();
};

#endif // GREENHOUSELOG_H
