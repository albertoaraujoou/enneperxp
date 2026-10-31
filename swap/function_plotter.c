#!/bin/bash

set -e

APP_NAME="FunctionPlotter"
APP_ID="functionplotter"
SOURCE_FILE="function_plotter.c"
ICON_FILE="icon.png"

echo "Compilando aplicación..."

if [ ! -f "$SOURCE_FILE" ]; then
    echo "ERROR: No existe $SOURCE_FILE"
    exit 1
fi

if [ ! -f "$ICON_FILE" ]; then
    echo "ERROR: No existe $ICON_FILE"
    exit 1
fi

gcc -O2 -std=c17 \
    "$SOURCE_FILE" \
    -o "$APP_NAME" \
    $(pkg-config --cflags --libs sdl2 SDL2_ttf) \
    -lm

APP_DIR="$HOME/.local/share/$APP_ID"

mkdir -p "$APP_DIR"
mkdir -p "$HOME/.local/share/icons/hicolor/256x256/apps"
mkdir -p "$HOME/.local/share/applications"

cp "$APP_NAME" "$APP_DIR/"

cp "$ICON_FILE" \
"$HOME/.local/share/icons/hicolor/256x256/apps/${APP_ID}.png"

cat > "$HOME/.local/share/applications/${APP_ID}.desktop" << EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=Function Plotter
Comment=Graficador matemático SDL2
Exec=$APP_DIR/$APP_NAME
Icon=$APP_ID
Terminal=false
Categories=Education;Science;Math;
StartupNotify=true
EOF

chmod +x "$APP_DIR/$APP_NAME"
chmod +x "$HOME/.local/share/applications/${APP_ID}.desktop"

update-desktop-database ~/.local/share/applications 2>/dev/null || true
gtk-update-icon-cache ~/.local/share/icons/hicolor 2>/dev/null || true

echo
echo "Aplicación instalada correctamente."
echo
echo "Ejecutable:"
echo "$APP_DIR/$APP_NAME"
echo
echo "Lanzador:"
echo "$HOME/.local/share/applications/${APP_ID}.desktop"
echo
echo "Ahora aparecerá en el menú de aplicaciones de Ubuntu."
