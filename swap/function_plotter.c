#!/bin/bash

set -e

APP_NAME="FunctionPlotter"
SOURCE="function_plotter.c"
ICON="icon.png"

echo "Compilando..."

gcc -O2 -std=c17 \
    "$SOURCE" \
    -o "$APP_NAME" \
    $(pkg-config --cflags --libs sdl2 SDL2_ttf) \
    -lm

echo "Creando estructura de aplicación..."

mkdir -p "${APP_NAME}.app"

cp "$APP_NAME" "${APP_NAME}.app/"
cp "$ICON" "${APP_NAME}.app/"

cat > "${APP_NAME}.app/${APP_NAME}.desktop" <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=Function Plotter
Comment=Graficador matematico SDL2
Exec=$(pwd)/${APP_NAME}.app/${APP_NAME}
Icon=$(pwd)/${APP_NAME}.app/${ICON}
Terminal=false
Categories=Education;Science;
EOF

chmod +x "${APP_NAME}.app/${APP_NAME}"
chmod +x "${APP_NAME}.app/${APP_NAME}.desktop"

echo
echo "Aplicacion creada:"
echo "$(pwd)/${APP_NAME}.app"
