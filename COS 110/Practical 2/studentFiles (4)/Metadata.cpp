#include "Metadata.h"

Metadata::Metadata()
{
    size = 0;
    description = "New file/folder";
    Date defaultDate = {10, 8, 2026};
    dateCreated = defaultDate;
    lastModified = defaultDate;
    permission = full;
}

Metadata::Metadata(int size, const string &description, Date dateCreated)
{

    this->size = size;
    this->description = description;
    this->dateCreated = dateCreated;
    this->lastModified = dateCreated;
    this->permission = full;
}

int Metadata::getSize() const
{

    return size;
}

const string &Metadata::getDescription() const
{

    return description;
}

Date Metadata::getDateCreated() const
{

    return dateCreated;
}

Date Metadata::getDateModified() const
{

    return lastModified;
}

Permission Metadata::getPermission() const
{

    return permission;
}

bool Metadata::setSize(int size)
{

    if (size < 0)
    {
        return false;
    }
    else
    {
        this->size = size;
        return true;
    }
}

void Metadata::setDescription(const string &description)
{

    this->description = description;
}

void Metadata::setDateModified(Date lastModified)
{

    if (isBefore(lastModified, this->dateCreated) || isBefore(lastModified, this->lastModified))
    {
        return;
    }
    else
    {
        this->lastModified = lastModified;
    }
}

void Metadata::setPermission(Permission permission)
{

    this->permission = permission;
}

Metadata::~Metadata()
{
}
