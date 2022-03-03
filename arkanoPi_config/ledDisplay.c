/** File Name          : arkanoPiLib.c
  * Description        : Subrutinas y rutinas de atención de la máquina de estados del led display y
  * 					 ejecución del programa
  * 					 Mejoras incluídas
  */

#include "ledDisplay.h"

tipo_pantalla pantalla_inicial = {
	.matriz = {
	{0,0,0,0,0,0,0,0},
	{0,1,1,0,0,1,1,0},
	{0,1,1,0,0,1,1,0},
	{0,0,0,0,0,0,0,0},
	{0,1,0,0,0,0,1,0},
	{0,0,1,1,1,1,0,0},
	{0,0,0,0,0,0,0,0},
	}
};

tipo_pantalla pantalla_final = {
	.matriz = {
	{0,0,0,0,0,0,0,0},
	{0,1,1,0,0,1,1,0},
	{0,1,1,0,0,1,1,0},
	{0,0,0,0,0,0,0,0},
	{0,0,1,1,1,1,0,0},
	{0,1,0,0,0,0,1,0},
	{0,0,0,0,0,0,0,0},
	}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_display[] = {
	{ DISPLAY_ESPERA_COLUMNA, CompruebaTimeoutColumnaDisplay, DISPLAY_ESPERA_COLUMNA, ActualizaExcitacionDisplay },
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...
	// ...
	tmr_startms(led_display->tmr_refresco_display, TIMEOUT_COLUMNA_DISPLAY);
	int i = 0;
	for (i=0; i<NUM_PINES_CONTROL_COLUMNAS_DISPLAY; i++){
		pinMode(led_display->pines_control_columnas[i], OUTPUT);
		digitalWrite(led_display->pines_control_columnas[i], LOW);
	}
	int j = 0;
	for (j=0; j<NUM_FILAS_DISPLAY; j++){
		pinMode(led_display->filas[j], OUTPUT);
		digitalWrite(led_display->filas[j], HIGH);
	}
}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ApagaFilas (TipoLedDisplay *led_display){
	// A completar por el alumno...
	// ...
	int i = 0;
	for(i = 0; i<NUM_FILAS_DISPLAY; i++){
		digitalWrite(led_display->filas[i], HIGH);
	}
}
void ExcitaColumnas(int columna) {
	switch(columna) {
		// A completar por el alumno...
		// ...
	case 0: //Se activa la primera columna del display
		digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
		digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
		digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
		break;

	case 1: //Se activa la segunda columna del display
		digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
		digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
		digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
		break;

	case 2: //Se activa la tercera columna del display
		digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
		digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
		digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
		break;

	case 3: //Se activa la cuarta columna del display
		digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
		digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
		digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
		break;

	case 4: //Se activa la quinta columna del display
		digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
		digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
		digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
		break;
	case 5: //Se activa la sexta columna del display
		digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
		digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
		digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
		break;
	case 6: //Se activa la séptima columna del display
		digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
		digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
		digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
		break;
	case 7: //Se activa la octava columna del display
		digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
		digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
		digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
		break;
	}
}

void ActualizaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...
	// ...
	ApagaFilas(led_display); //p_columna es la columna actual!!!

	if (led_display->p_columna >= (NUM_COLUMNAS_DISPLAY -1)){
		led_display->p_columna = 0;
	}else{
		led_display->p_columna++;
	}

	ExcitaColumnas(led_display->p_columna);

	//Ahora excitamos filas:
	int columna_actual= led_display->p_columna;
	int i=0;
	for ( i = 0; i < NUM_FILAS_DISPLAY; i++){
		if (led_display->pantalla.matriz[i][columna_actual] == 1 || led_display->pantalla.matriz[i][columna_actual] == 8 ){ //Comprobamos si a ese led le toca estar a 1
			digitalWrite(led_display->filas[i], LOW); //Lo activamos con LOW (activo a nivel bajo)
		}
	}
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumnaDisplay (fsm_t* this) {
	int result = 0;
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);

	// A completar por el alumno...
	// ...

	piLock (MATRIX_KEY);
	result = (p_ledDisplay->flags & FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock (MATRIX_KEY);
	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ActualizaExcitacionDisplay (fsm_t* this) {
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);

	// A completar por el alumno...
	// ...

	tmr_startms(led_display.tmr_refresco_display, TIMEOUT_COLUMNA_DISPLAY);
	piLock (MATRIX_KEY);
	p_ledDisplay->flags &= (~FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock (MATRIX_KEY);
	ActualizaLedDisplay(p_ledDisplay);
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_refresco_display_isr (union sigval value) {
	// A completar por el alumno...
	// ...
	piLock(MATRIX_KEY);
	led_display.flags|= FLAG_TIMEOUT_COLUMNA_DISPLAY;
	piUnlock(MATRIX_KEY);
}
