#!/bin/bash


# Colores
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

echo "=== TESTING MINISHELL QUOTE HANDLING ==="

# Función para probar un comando
test_command() {
    local cmd="$1"
    local description="$2"
    
    echo -e "\n--- $description ---"
    echo "Command: $cmd"
    
    # Bash
    bash_output=$(echo "$cmd" | bash 2>&1)
    bash_exit=$?
    
    # Minishell - CAPTURA TODO y luego EXTRAE el output
    full_minishell_output=$(echo "$cmd" | timeout 5 ./minishell 2>&1)
    minishell_exit=$?
    
    # Extraer solo la parte entre el comando y el siguiente prompt
    minishell_output=$(echo "$full_minishell_output" | sed -n "/$cmd/{n;p;}" | head -n 1)
    
    # Si no funciona, método alternativo: quitar primeros y últimos renglones
    if [ -z "$minishell_output" ]; then
        line_count=$(echo "$full_minishell_output" | wc -l)
        if [ $line_count -gt 2 ]; then
            minishell_output=$(echo "$full_minishell_output" | tail -n +2 | head -n -1)
        else
            minishell_output="$full_minishell_output"
        fi
    fi
    
    # Limpiar espacios
    bash_clean=$(echo "$bash_output" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
    minishell_clean=$(echo "$minishell_output" | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
    
    # Comparar
    if [ "$bash_clean" = "$minishell_clean" ] && [ $bash_exit -eq $minishell_exit ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        echo "Bash: '$bash_clean'"
        echo "Mini: '$minishell_clean'"
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "Bash: '$bash_clean'"
        echo "Mini: '$minishell_clean'" 
        echo "Full Mini: '$full_minishell_output'"
    fi
}
# Tests básicos de comillas simples
test_command "echo 'hello world'" "Comillas simples básicas"
test_command "echo 'hello''world'" "Comillas simples concatenadas"
test_command "echo 'hello\$USER'" "Comillas simples bloquean \$"
test_command "echo 'hello|world'" "Comillas simples bloquean pipe"
test_command "echo 'hello>world'" "Comillas simples bloquean redirección"

# Tests básicos de comillas dobles  
test_command 'echo "hello world"' "Comillas dobles básicas"
test_command 'echo "hello""world"' "Comillas dobles concatenadas"
test_command 'echo "hello\$USER"' "Comillas dobles expanden \$"
test_command 'echo "hello|world"' "Comillas dobles bloquean pipe"
test_command 'echo "hello>world"' "Comillas dobles bloquean redirección"

# Tests mixtos
test_command "echo 'hello'\"world\"" "Comillas simples + dobles"
test_command 'echo "hello'"'"'world"' "Comillas dobles con simples inside"
test_command "echo 'hello\"world'" "Comillas simples con dobles inside"

# Variables y comillas
test_command 'echo "$USER"' "Variable expande sin comillas"
test_command "echo '\$USER'" "Variable NO expande en simples"
test_command 'echo "$USERtest"' "Variable sin llaves"
test_command 'echo "${USER}test"' "Variable con llaves"

# Comillas sin cerrar
test_command 'echo "hello' "Comillas dobles sin cerrar"
test_command "echo 'hello" "Comillas simples sin cerrar"

# Caracteres especiales
test_command 'echo "hello\\nworld"' "Comillas dobles con \\n"
test_command "echo 'hello\\nworld'" "Comillas simples con \\n"
test_command 'echo "hello\\tworld"' "Comillas dobles con \\t"

# Múltiples espacios y comillas
test_command 'echo "hello   world"' "Espacios preservados en dobles"
test_command "echo 'hello   world'" "Espacios preservados en simples"

# Comillas vacías
test_command 'echo ""' "Comillas dobles vacías"
test_command "echo ''" "Comillas simples vacías"
test_command 'echo "" ""' "Múltiples comillas vacías"

# Combinaciones complejas
test_command 'echo "hello"'"'world'" "Alternando comillas"
test_command "echo '\"hello\"'\"'world'\"" "Comillas anidadas complejas"

# Con otros comandos
test_command 'cat << EOF | echo "test"' "Heredoc con comillas"
test_command 'echo "hello" | cat' "Pipe con comillas"

echo -e "\n=== TESTING COMPLETE ==="