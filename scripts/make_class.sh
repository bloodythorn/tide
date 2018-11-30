#! /bin/bash

# This script given a name and a location will copy the class template out of
# PROJECT_ROOT_DIR/snippets/class.??? rename all instances of 'class' to the
# name given, and create the file in the given location.
#

# Check to make sure name is there and valid
if [ "$1" == "" ]
  then
    echo "No Name!"
    exit 1
fi

NAME="$1"
LNAME=$(echo $NAME | tr '[:upper:]' '[:lower:]')
UNAME=$(echo $NAME | tr '[:lower:]' '[:upper:]')
CLASS="Class"
LCLASS=$(echo $CLASS | tr '[:upper:]' '[:lower:]')
UCLASS=$(echo $CLASS | tr '[:lower:]' '[:upper:]')
CHPP="$(cat ../snippets/class.hpp)"
CCPP="$(cat ../snippets/class.cpp)"

ONE="${CHPP//"$CLASS"/$NAME}"
NHPP="${ONE//"$UCLASS"/$UNAME}"
ONE="${CCPP//"$CLASS"/$NAME}"
TWO="${ONE//"$LCLASS"/$LNAME}"
NCPP="${TWO//"$UCLASS"/$UNAME}"
$(echo "$NHPP" > "$LNAME.hpp")
$(echo "$NCPP" > "$LNAME.cpp")
