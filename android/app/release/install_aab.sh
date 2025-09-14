#!/bin/bash

# Rutas
AAB_PATH="/Users/hakkindavid/Documents/GitHub/fractochales/android/app/release/app-release.aab"
BUNDLETOOL="/Users/hakkindavid/dev/bundletool.jar"
KEYSTORE="/Users/hakkindavid/hakkindavid_key_android"
KEY_ALIAS="hakkindavid_key_android"

# Pedir contraseñas de forma segura
read -s -p "Contraseña del keystore: " KS_PASS
echo
read -s -p "Contraseña de la key: " KEY_PASS
echo

# Generar APKS
java -jar "$BUNDLETOOL" build-apks \
  --bundle="$AAB_PATH" \
  --output="out.apks" \
  --ks="$KEYSTORE" \
  --ks-pass=pass:"$KS_PASS" \
  --ks-key-alias="$KEY_ALIAS" \
  --key-pass=pass:"$KEY_PASS"

# Desinstalar app previa
adb uninstall dev.bonsanbec.fractochales

# Instalar el APKS generado
java -jar "$BUNDLETOOL" install-apks --apks="out.apks"

echo "Instalación completada. Verifica la app en tu dispositivo."
