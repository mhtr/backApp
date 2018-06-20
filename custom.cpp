#include "custom.hpp"

#include <QTextStream>
#include <QFile>

#include <iostream>
#include <filesystem>

Custom::Custom(QString filename) : filename(filename)
{

}

std::filesystem::path Custom::parseDst() const
{
    QTextStream stream;
    std::filesystem::path dst;

    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        stream.setDevice(&file);

        while (!stream.atEnd())
        {
            QString line = stream.readLine().trimmed();

            if (line.startsWith("dst="))
            {
                dst = line.remove(0, 4).toStdWString();
            }
        }

        file.close();
    }
    else
    {
        std::cout << "Unable to open file!" << std::endl;
        std::exit(-1);
    }

    return dst;
}

std::filesystem::path  Custom::parseSrc() const
{
    QTextStream stream;
    std::filesystem::path src;

    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        stream.setDevice(&file);

        while (!stream.atEnd())
        {
            QString line = stream.readLine().trimmed();

            if (line.startsWith("src="))
            {
                src = line.remove(0, 4).toStdWString();
            }
        }

        file.close();
    }
    else
    {
        std::cout << "Unable to open file!" << std::endl;
        std::exit(-1);
    }

    return src;
}

int Custom::parseTtl() const
{
    QTextStream stream;
    int ttl = 0;

    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        stream.setDevice(&file);

        while (!stream.atEnd())
        {
            QString line = stream.readLine().trimmed();

            if (line.startsWith("ttl="))
            {
                ttl = line.remove(0, 4).toInt();
            }
        }

        file.close();
    }
    else
    {
        std::cout << "Unable to open file!" << std::endl;
        std::exit(-1);
    }

    return ttl;
}
