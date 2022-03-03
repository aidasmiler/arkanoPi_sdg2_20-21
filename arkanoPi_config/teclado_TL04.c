/** File Name          : teclado_TL04.c
  * Description        : Subrutinas y rutinas de atención de la máquina de estados del teclado y
  * 					 ejecución del programa
  * 					 Mejoras incluídas
  */

#include "teclado_TL04.h"

char tecladoTL04[4][4] = {
	{'1', '2', '3', 'C'},
	{'4', '5', '6', 'D'},
	{'7', '8', '9', 'E'},
	{'A', '0', 'B', 'F'}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_columnas[] = {
	{ TECLADO_ESPERA_COLUMNA, CompruebaTimeoutColumna, TECLADO_ESPERA_COLUMNA, TecladoExcitaColumna },
	{-1, NULL, -1, NULL },
};

fsm_trans_t fsm_trans_deteccion_pulsaciones[] = {
	{ TECLADO_ESPERA_TECLA, CompruebaTeclaPulsada, TECLADO_ESPERA_TECLA, ProcesaTeclaPulsada},
	{-1, NULL, -1, NULL },
};


//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTeclado(TipoTeclado *p_teclado) {
	// A completar por el alumno...
	// ...
	int j = 0;
	for (j=0; j<NUM_FILAS_TECLADO; j++){
	p_teclado->debounceTime[j] = 0;
	}
	p_teclado->teclaPulsada.col = -1;
	p_teclado->teclaPulsada.row = -1;
	p_teclado->columna_actual = COLUMNA_1;
	p_teclado->flags = 0;

	int i=0;
	for (i=0; i<4; i++){
		pinMode(p_teclado->columnas[i], OUTPUT);
		pinMode(p_teclado->filas[i], INPUT);
		pullUpDnControl (p_teclado->filas[i], PUD_DOWN );  // todas las filas inicialmente a pull down
		wiringPiISR(p_teclado->filas[i], INT_EDGE_RISING, p_teclado->rutinas_ISR[i]);  //array de rutinas de interrupciï¿½n,
		// detectan flanco de subida
		}
	digitalWrite(teclado.columnas[0], HIGH);
	int k = 0;
	for (k=1; k<4; k++){
		digitalWrite(teclado.columnas[k], LOW);
	}
	tmr_startms(p_teclado->tmr_duracion_columna, TIMEOUT_COLUMNA_TECLADO);
}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ActualizaExcitacionTecladoGPIO (int columna) {
	// A completar por el alumno
	// ...

	tmr_startms(teclado.tmr_duracion_columna, TIMEOUT_COLUMNA_TECLADO);
	switch(columna){
		// ...
	case COLUMNA_1:
		digitalWrite(GPIO_KEYBOARD_COL_1, HIGH);
		digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
		digitalWrite(GPIO_KEYBOARD_COL_3, LOW);
		digitalWrite(GPIO_KEYBOARD_COL_4, LOW);
		break;

	case COLUMNA_2:
		digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
		digitalWrite(GPIO_KEYBOARD_COL_2, HIGH);
		digitalWrite(GPIO_KEYBOARD_COL_3, LOW);
		digitalWrite(GPIO_KEYBOARD_COL_4, LOW);
		break;

	case COLUMNA_3:
		digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
		digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
		digitalWrite(GPIO_KEYBOARD_COL_3, HIGH);
		digitalWrite(GPIO_KEYBOARD_COL_4, LOW);
		break;

	case COLUMNA_4:
		digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
		digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
		digitalWrite(GPIO_KEYBOARD_COL_3, LOW);
		digitalWrite(GPIO_KEYBOARD_COL_4, HIGH);
		break;
	default:
		break;
	}
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumna (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno...
	// ...

	piLock (KEYBOARD_KEY);
	result = (p_teclado->flags & FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock (KEYBOARD_KEY);

	return result;
}

int CompruebaTeclaPulsada (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock (KEYBOARD_KEY);
	result = (p_teclado->flags & FLAG_TECLA_PULSADA);
	piUnlock (KEYBOARD_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LAS MAQUINAS DE ESTADOS
//------------------------------------------------------

void TecladoExcitaColumna (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...
	p_teclado = &teclado;
	piLock (KEYBOARD_KEY);
	p_teclado->flags &= (~FLAG_TIMEOUT_COLUMNA_TECLADO);

	p_teclado->columna_actual++;

	if(p_teclado->columna_actual >= 4){
		p_teclado->columna_actual = COLUMNA_1;
	}
	// Llamada a ActualizaExcitacionTecladoGPIO con columna a activar como argumento
	ActualizaExcitacionTecladoGPIO(p_teclado->columna_actual);
	piUnlock (KEYBOARD_KEY);

}

void ProcesaTeclaPulsada (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...

	piLock (KEYBOARD_KEY);
	p_teclado->flags &= (~FLAG_TECLA_PULSADA);
	switch(p_teclado->teclaPulsada.col){ //metemos todas las casuísticas porque no sabemos por qué el default ha decidido que no va a funcionar
		case COLUMNA_1:

			if(p_teclado->teclaPulsada.row == FILA_1){
				//Caso tecla D
				printf("\nKey pressed D!!!\n");
				printf("Wrong key pressed!!!\n");
				printf("1 = Comienza el juego lento\n");
				printf("3 = Comienza el juego rápido\n");
				printf("A = Reinicia el juego\n");
				printf("4 = Mueve Izquierda\n");
				printf("6 = Mueve Derecha\n");
				printf("0 = Pausa el juego\n");
				printf("# = Reanuda el juego\n");
				printf("C = Acaba el juego\n");
			}else if(p_teclado->teclaPulsada.row == FILA_2){
				//Caso tecla C
				//Funcionamiento básico del juego
				printf("\nKey pressed C!!!\n");
				printf("\nGame Over!!!\n");
				piLock (SYSTEM_FLAGS_KEY);
				flags|= FLAG_FIN_JUEGO;
				piUnlock (SYSTEM_FLAGS_KEY);
			}else if(p_teclado->teclaPulsada.row == FILA_3){
				//Caso tecla B
				printf("\nKey pressed B!!!\n");
				printf("Wrong key pressed!!!\n");
				printf("1 = Comienza el juego lento\n");
				printf("3 = Comienza el juego rápido\n");
				printf("A = Reinicia el juego\n");
				printf("4 = Mueve Izquierda\n");
				printf("6 = Mueve Derecha\n");
				printf("0 = Pausa el juego\n");
				printf("# = Reanuda el juego\n");
				printf("C = Acaba el juego\n");
			}else if(p_teclado->teclaPulsada.row == FILA_4){
				//Caso tecla A
				//Funcionamiento básico del juego
				printf("\nKey pressed A!!!\n");
				printf("Se reinicia el juego!!!\n");
				printf("TIENES 3 VIDAS INCIALES!\n");
				piLock (SYSTEM_FLAGS_KEY);
				flags|= FLAG_BOTON;
				piUnlock (SYSTEM_FLAGS_KEY);
			}
			break;
		case COLUMNA_2:
			if(p_teclado->teclaPulsada.row == FILA_1){
				//Caso tecla #
				//MEJORA 4
				printf("\nKey pressed #!!!\n");
				printf("Se reanuda el juego\n");
				piLock (SYSTEM_FLAGS_KEY);
				flags|= FLAG_REANUDA;
				piUnlock (SYSTEM_FLAGS_KEY);
			}else if(p_teclado->teclaPulsada.row == FILA_2){
				//Caso tecla 9
				printf("\nKey pressed 9!!!\n");
				printf("Wrong key pressed!!!\n");
				printf("1 = Comienza el juego lento\n");
				printf("3 = Comienza el juego rápido\n");
				printf("A = Reinicia el juego\n");
				printf("4 = Mueve Izquierda\n");
				printf("6 = Mueve Derecha\n");
				printf("0 = Pausa el juego\n");
				printf("# = Reanuda el juego\n");
				printf("C = Acaba el juego\n");
			}else if(p_teclado->teclaPulsada.row == FILA_3){
				//Caso tecla 6
				//Funcionamiento básico del juego
				printf("\nKey pressed 6!!!\n");
				printf("\nMovemos a la derecha!!!\n");
				piLock (SYSTEM_FLAGS_KEY);
				flags|= FLAG_MOV_DERECHA;
				piUnlock (SYSTEM_FLAGS_KEY);

			}else if(p_teclado->teclaPulsada.row == FILA_4){
				//Caso tecla 3
				//MEJORA 5
				printf("\nKey pressed 3!!!\n");
				printf("\nEl juego va rápido!!! WATCH OUT\n");
				printf("TIENES 3 VIDAS INCIALES!\n");
				piLock (SYSTEM_FLAGS_KEY);
				flags|= FLAG_RAPIDO;
				piUnlock (SYSTEM_FLAGS_KEY);
			}
			break;
		case COLUMNA_3:
			if(p_teclado->teclaPulsada.row == FILA_1){
				//Caso tecla 0
				//MEJORA 4
				printf("\nKey pressed 0!!!\n");
				printf("Se ha pausado el juego\n");
				piLock (SYSTEM_FLAGS_KEY);
				flags|= FLAG_PAUSA;
				piUnlock (SYSTEM_FLAGS_KEY);
			}else if(p_teclado->teclaPulsada.row == FILA_2){
				//Caso tecla 8
				printf("\nKey pressed 8!!!\n");
				printf("Wrong key pressed!!!\n");
				printf("1 = Comienza el juego lento\n");
				printf("3 = Comienza el juego rápido\n");
				printf("A = Reinicia el juego\n");
				printf("4 = Mueve Izquierda\n");
				printf("6 = Mueve Derecha\n");
				printf("0 = Pausa el juego\n");
				printf("# = Reanuda el juego\n");
				printf("C = Acaba el juego\n");
			}else if(p_teclado->teclaPulsada.row == FILA_4){
				//Caso tecla 2
				printf("\nKey pressed 2!!!\n");
				printf("Wrong key pressed!!!\n");
				printf("1 = Comienza el juego lento\n");
				printf("3 = Comienza el juego rápido\n");
				printf("A = Reinicia el juego\n");
				printf("4 = Mueve Izquierda\n");
				printf("6 = Mueve Derecha\n");
				printf("0 = Pausa el juego\n");
				printf("# = Reanuda el juego\n");
				printf("C = Acaba el juego\n");
			}else if(p_teclado->teclaPulsada.row == FILA_3){
				//Caso tecla 5
				printf("\nKey pressed 5!!!\n");
				printf("Wrong key pressed!!!\n");
				printf("1 = Comienza el juego lento\n");
				printf("3 = Comienza el juego rápido\n");
				printf("A = Reinicia el juego\n");
				printf("4 = Mueve Izquierda\n");
				printf("6 = Mueve Derecha\n");
				printf("0 = Pausa el juego\n");
				printf("# = Reanuda el juego\n");
				printf("C = Acaba el juego\n");
			}
			break;
		case COLUMNA_4:
			if(p_teclado->teclaPulsada.row == FILA_1){
				//Caso tecla *
				printf("\nKey pressed *!!!\n");
				printf("Wrong key pressed!!!\n");
				printf("1 = Comienza el juego lento\n");
				printf("3 = Comienza el juego rápido\n");
				printf("A = Reinicia el juego\n");
				printf("4 = Mueve Izquierda\n");
				printf("6 = Mueve Derecha\n");
				printf("0 = Pausa el juego\n");
				printf("# = Reanuda el juego\n");
				printf("C = Acaba el juego\n");
			}else if(p_teclado->teclaPulsada.row == FILA_2){
				//Caso tecla 7
				printf("\nKey pressed 7!!!\n");
				printf("Wrong key pressed!!!\n");
				printf("1 = Comienza el juego lento\n");
				printf("3 = Comienza el juego rápido\n");
				printf("A = Reinicia el juego\n");
				printf("4 = Mueve Izquierda\n");
				printf("6 = Mueve Derecha\n");
				printf("0 = Pausa el juego\n");
				printf("# = Reanuda el juego\n");
				printf("C = Acaba el juego\n");
			}else if(p_teclado->teclaPulsada.row == FILA_3){
				//Caso tecla 4
				//Funcionamiento básico del juego
				printf("\nKey pressed 4!!!\n");
				printf("\nMovemos a la izquierda!!!\n");
				piLock (SYSTEM_FLAGS_KEY);
				flags|= FLAG_MOV_IZQUIERDA;
				piUnlock (SYSTEM_FLAGS_KEY);
			}else if(p_teclado->teclaPulsada.row == FILA_4){
				//Caso tecla 1
				//MEJORA 5
				printf("\nKey pressed 1!!!\n");
				printf("\nEl juego va despacito biennn\n");
				printf("TIENES 3 VIDAS INCIALES!\n");
				piLock (SYSTEM_FLAGS_KEY);
				flags|= FLAG_LENTO;
				piUnlock (SYSTEM_FLAGS_KEY);
			}
		break;
		default:
			printf("Wrong key pressed!!!\n");
			printf("1 = Comienza el juego lento\n");
			printf("3 = Comienza el juego rápido\n");
			printf("A = Reinicia el juego\n");
			printf("4 = Mueve Izquierda\n");
			printf("6 = Mueve Derecha\n");
			printf("0 = Pausa el juego\n");
			printf("# = Reanuda el juego\n");
			printf("C = Acaba el juego\n");
			p_teclado->teclaPulsada.row = -1;
			p_teclado->teclaPulsada.col = -1;
			break;
	}

	piUnlock (KEYBOARD_KEY);
}


//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void teclado_fila_1_isr (void) {
	// A completar por el alumno
	// ...
	if(millis()<teclado.debounceTime[FILA_1]) {
		teclado.debounceTime[FILA_1] = millis() + DEBOUNCE_TIME;
		return;
	}
	piLock(KEYBOARD_KEY);
	teclado.teclaPulsada.row = FILA_1;
	teclado.teclaPulsada.col = teclado.columna_actual;
	teclado.flags|= FLAG_TECLA_PULSADA;
	piUnlock(KEYBOARD_KEY);

	teclado.debounceTime[FILA_1]=millis() + DEBOUNCE_TIME;

}

void teclado_fila_2_isr (void) {
	// A completar por el alumno
	// ...
	if(millis()<teclado.debounceTime[FILA_2]) {
		teclado.debounceTime[FILA_2] = millis() + DEBOUNCE_TIME;
		return;
	}
	piLock(KEYBOARD_KEY);
	teclado.teclaPulsada.row = FILA_2;
	teclado.teclaPulsada.col = teclado.columna_actual;
	teclado.flags|= FLAG_TECLA_PULSADA;
	piUnlock(KEYBOARD_KEY);

	teclado.debounceTime[FILA_2]=millis() + DEBOUNCE_TIME;

}

void teclado_fila_3_isr (void) {
	// A completar por el alumno
	// ...
	if(millis()<teclado.debounceTime[FILA_3]) {
		teclado.debounceTime[FILA_3] = millis() + DEBOUNCE_TIME;
		return;
	}
	piLock(KEYBOARD_KEY);
	teclado.teclaPulsada.row = FILA_3;
	teclado.teclaPulsada.col = teclado.columna_actual;
	teclado.flags|= FLAG_TECLA_PULSADA;
	piUnlock(KEYBOARD_KEY);

	teclado.debounceTime[FILA_3]=millis() + DEBOUNCE_TIME;

}

void teclado_fila_4_isr (void) {
	// A completar por el alumno
	// ...
	if(millis()<teclado.debounceTime[FILA_4]) {
		teclado.debounceTime[FILA_4] = millis()+DEBOUNCE_TIME ;
		return;
	}
	piLock(KEYBOARD_KEY);
	teclado.teclaPulsada.row = FILA_4;
	teclado.teclaPulsada.col = teclado.columna_actual;
	teclado.flags|= FLAG_TECLA_PULSADA;
	piUnlock(KEYBOARD_KEY);

	teclado.debounceTime[FILA_4]=millis() + DEBOUNCE_TIME;

}

void timer_duracion_columna_isr (union sigval value) {
	// A completar por el alumno
	// ...
	piLock(KEYBOARD_KEY);
	teclado.flags|= FLAG_TIMEOUT_COLUMNA_TECLADO;
	piUnlock(KEYBOARD_KEY);
}
