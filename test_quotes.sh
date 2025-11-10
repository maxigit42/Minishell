
#!/bin/bash

# Script de pruebas para comparar tu minishell con bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

MINISHELL="./minishell"
TESTFILE="/tmp/minishell_test_$$"
BASHFILE="/tmp/bash_test_$$"

passed=0
failed=0

# Función para ejecutar test
run_test() {
    local test_name="$1"
    local command="$2"
    
    echo -e "\n${YELLOW}TEST: $test_name${NC}"
    echo "Comando: $command"
    
    # Ejecutar en bash
    bash -c "$command" > "$BASHFILE" 2>&1
    bash_exit=$?
    
    # Ejecutar en minishell
    echo "$command" | $MINISHELL > "$TESTFILE" 2>&1
    mini_exit=$?
    
    # Comparar salidas
    if diff -q "$BASHFILE" "$TESTFILE" > /dev/null 2>&1; then
        if [ $bash_exit -eq $mini_exit ]; then
            echo -e "${GREEN}✓ PASS${NC}"
            ((passed++))
        else
            echo -e "${RED}✗ FAIL (exit status)${NC}"
            echo "Bash exit: $bash_exit | Minishell exit: $mini_exit"
            ((failed++))
        fi
    else
        echo -e "${RED}✗ FAIL (output)${NC}"
        echo "=== BASH OUTPUT ==="
        cat "$BASHFILE"
        echo "=== MINISHELL OUTPUT ==="
        cat "$TESTFILE"
        echo "==================="
        ((failed++))
    fi
}

# TESTS DE EXPANSIÓN
echo "========================================="
echo "TESTS DE EXPANSIÓN DE VARIABLES"
echo "========================================="

export TEST_VAR="hello"

run_test "Expansión básica" 'echo $TEST_VAR'
run_test "Sin expansión en comillas simples" "echo '\$TEST_VAR'"
run_test "Con expansión en comillas dobles" 'echo "$TEST_VAR"'
run_test "Comillas simples dentro de dobles" 'echo "'\''$TEST_VAR'\''"'
run_test "Comillas dobles dentro de simples" "echo '\"$TEST_VAR\"'"
run_test "Expansión de \$?" 'ls /nonexistent 2>/dev/null; echo $?'
run_test "Dollar solo" 'echo $'
run_test "Dollar con dígito" 'echo $1 $2'
run_test "Dollar con carácter inválido" 'echo $@ $*'
run_test "Variable inexistente" 'echo $NONEXISTENT_VAR'
run_test "Variable pegada a texto" 'echo $TEST_VARextra'

# TESTS DE COMILLAS
echo -e "\n========================================="
echo "TESTS DE COMILLAS"
echo "========================================="

run_test "Comillas simples vacías" "echo ''"
run_test "Comillas dobles vacías" 'echo ""'
run_test "Comillas mixtas" "echo 'hola' \"mundo\""
run_test "Espacios dentro de comillas" 'echo "  espacios  "'
run_test "Tabs dentro de comillas" 'echo "	tabs	"'

# TESTS DE ERRORES DE SINTAXIS
echo -e "\n========================================="
echo "TESTS DE ERRORES DE SINTAXIS"
echo "========================================="

# Nota: estos tests verifican que minishell NO crashee
echo "echo \"sin cerrar" | $MINISHELL > /dev/null 2>&1
if [ $? -eq 2 ]; then
    echo -e "${GREEN}✓ Comillas sin cerrar: exit 2${NC}"
    ((passed++))
else
    echo -e "${RED}✗ Comillas sin cerrar: exit $?${NC}"
    ((failed++))
fi

# TESTS DE PIPES CON EXPANSIÓN
echo -e "\n========================================="
echo "TESTS DE PIPES + EXPANSIÓN"
echo "========================================="

run_test "Pipe con expansión" 'echo "$TEST_VAR" | cat'
run_test "Pipe sin expansión" "echo '\$TEST_VAR' | cat"

# RESUMEN
echo -e "\n========================================="
echo "RESUMEN"
echo "========================================="
echo -e "${GREEN}Tests pasados: $passed${NC}"
echo -e "${RED}Tests fallidos: $failed${NC}"

# Cleanup
rm -f "$TESTFILE" "$BASHFILE"

if [ $failed -eq 0 ]; then
    echo -e "\n${GREEN}¡Todos los tests pasaron!${NC}"
    exit 0
else
    echo -e "\n${RED}Algunos tests fallaron. Revisa la salida arriba.${NC}"
    exit 1
fi