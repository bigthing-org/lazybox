#include "commonheader.h"
#include "lazyboxconsole.h"

#include "softcleaner/communicator.hpp"
#include "softcleaner/downloader.hpp"
#include "softcleaner/emailer.hpp"
#include "softcleaner/game.hpp"
#include "softcleaner/ime.hpp"
#include "softcleaner/industrytool.hpp"
#include "softcleaner/programmer.hpp"
#include "softcleaner/reader.hpp"
#include "softcleaner/system.hpp"
#include "softcleaner/systool.hpp"
#include "softcleaner/videoplayer.hpp"
#include "softcleaner/webbrowser.hpp"

#include "datarecovery/partition.h"

lazyboxconsole::lazyboxconsole()

{
}

void handleDisk(QTextStream& cout, QStringList& params)
{
    QString subparam = params[1];
    if (subparam == "show")
    {
        QVector<QString> target;
        QString t;
        if (params.size() > 2)
        {
            QString partition_file_path = params[2];
            target.push_back(partition_file_path);
        }

        QVector<QDisk> disks;
        QDiskUtils::GetPhysicalDrives(disks, target);

        t.sprintf("Disks:\n  indx  type      size(MB)  partnum  start(MB)\n");
        cout << t;

        for (int i = 0; i < disks.size(); i++)
        {
            t.sprintf("  %-4d  %-8s  %-8d  %-4d", i, QDiskUtils::GetTypeStr(disks[i].type),
                      int(disks[i].size / 1024 / 1024), disks[i].partitions.size());
            cout << t << endl;
            QDisk& disk = disks[i];
            for (int j = 0; j < disk.partitions.size(); j++)
            {
                t.sprintf("  --%-2d  %-8s  %-8d  %-4d     %-8d", j,
                          QDiskUtils::GetTypeStr(disk.partitions[j]->type),
                          int(disk.partitions[j]->size / 1024 / 1024), 0,
                          int(disk.partitions[j]->begin / 1024 / 1024));
                cout << t << endl;
            }
        }
    }
    else if (subparam == "showfile")
    {
        QString path = "/"; // Default is Root
        if (params.size() < 4)
        {
            cout << "Param format error" << endl;
            return;
        }
        if (params.size() >= 5)
        {
            path = params[4];
        }
        unsigned int disknum = params[2].toUInt();
        unsigned int partnum = params[3].toUInt();
        QVector<QString> target;
        if (params.size() >= 6)
        {
            QString partition_file_path = params[5];
            target.push_back(partition_file_path);
        }
        QVector<QDisk> disks;
        QDiskUtils::GetPhysicalDrives(disks, target);
        if (disks.size() <= disknum || disks[disknum].partitions.size() <= partnum)
        {
            cout << "Disk or partition index error" << endl;
            return;
        }
        QPartition* part = disks[disknum].partitions[partnum];
        cout << "Files in " << path << ":" << endl;
        for (QRecordFile* f : part->ListFiles(path))
        {
            cout << "  " << f->GetName() << endl;
        }
    }

}

int lazyboxconsole::loop()
{
    QTextStream cin(stdin, QIODevice::ReadOnly);
    QTextStream cout(stdout, QIODevice::WriteOnly);
    QTextStream cerr(stderr, QIODevice::WriteOnly);
    QString input;
    cout << "LazyBox " << LAZYBOX_VERSION << endl;
    cout << "Support Commands:" << endl;
    cout << "  quit                         quit this programe" << endl;
    cout << "  disk show [SP]               show filesystems on disks" << endl;
    cout << "                               SP:     FileSystem Database(exp:1.iso)" << endl;
    cout << "  disk showfile M N FP [SP]    show file on disk M and partition N under File Path" << endl;
    cout << "                               M:      Disk Index" << endl;
    cout << "                               N:      Partition Index" << endl;
    cout << "                               SP:     FileSystem Database" << endl;
    cout << "                               FP:     Filepath like /user/tmp" << endl;

    while ((input = cin.readLine()) != "quit")
    {
        QStringList params = input.split(" ");
        if (params[0] == "disk" && params.length() > 1)
        {
            handleDisk(cout, params);
        }
    }
    return 0;
}
