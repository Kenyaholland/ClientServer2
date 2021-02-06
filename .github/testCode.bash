if grep -q socket src/main.cpp; then
    echo socket call found
else
    echo socket call not found
    exit 1
fi

if grep -q gethostbyname src/main.cpp; then
    echo gethostbyname call found
else
    echo gethostbyname call not found
    exit 1
fi

if grep -q connect src/main.cpp; then
    echo connect call found
else
    echo connect call not found
    exit 1
fi

if grep -q recv src/main.cpp; then
    echo recv call found
else
    echo recv call not found
    exit 1
fi

if grep -q send src/main.cpp; then
    echo send call found
else
    echo send call not found
    exit 1
fi
