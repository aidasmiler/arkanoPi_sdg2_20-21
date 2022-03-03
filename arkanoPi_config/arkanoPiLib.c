/** File Name          : arkanoPiLib.c
  * Description        : Subrutinas y rutinas de atención de la máquina de estados y
  * 					 ejecución del programa
  * 					 Mejoras incluídas
  */

#include "arkanoPiLib.h"
//------------------------------------------------------
// DIFERENTES PANTALLAS DEL JUEGO
//------------------------------------------------------
int ladrillos_basico[NUM_FILAS_DISPLAY][NUM_COLUMNAS_DISPLAY] = { //Pantalla de juego inicial
	{0,0,1,1,1,1,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
};
//MEJORA VIDAS (6)
tipo_pantalla dos= {
	.matriz = {
		{0,0,0,0,0,0,0,0},
		{0,0,1,1,1,1,0,0},
		{0,1,1,0,0,1,1,0},
		{0,0,0,0,0,1,1,0},
		{0,0,1,1,1,1,0,0},
		{0,1,1,0,0,0,0,0},
		{0,1,1,1,1,1,1,0},
		}
};
tipo_pantalla uno= {
	.matriz = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,1,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,1,1,1,1,0,0},
	{0,0,0,0,0,0,0,0},
		}
};
tipo_pantalla cero= {
	.matriz = {
	{0,0,0,1,1,0,0,0},
	{0,0,1,1,1,1,0,0},
	{0,1,1,0,0,1,1,0},
	{0,1,1,0,0,1,1,0},
	{0,1,1,0,0,1,1,0},
	{0,0,1,1,1,1,0,0},
	{0,0,0,1,1,0,0,0},
		}
};
//------------------------------------------------------
// FUNCIONES DE VISUALIZACION (ACTUALIZACION DEL OBJETO PANTALLA QUE LUEGO USARA EL DISPLAY)
//------------------------------------------------------

void PintaMensajeInicialPantalla (tipo_pantalla *p_pantalla, tipo_pantalla *p_pantalla_inicial) {
	int i, j = 0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_pantalla->matriz[i][j] = p_pantalla_inicial->matriz[i][j];
		}
	}

	return;
}

void PintaPantallaPorTerminal (tipo_pantalla *p_pantalla) {
#ifdef __SIN_PSEUDOWIRINGPI__
	int i=0, j=0;

	printf("\n[PANTALLA]\n");
	fflush(stdout);
	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			printf("%d", p_pantalla->matriz[i][j]);
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);
	}
	fflush(stdout);
#endif
}

void ReseteaPantalla (tipo_pantalla *p_pantalla) {
	int i=0, j=0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_pantalla->matriz[i][j] = 0;
		}
	}
}


//------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET
//------------------------------------------------------

void InicializaLadrillos(tipo_pantalla *p_ladrillos) {
	int i=0, j=0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_ladrillos->matriz[i][j] = ladrillos_basico[i][j];
		}
	}
}

void InicializaPelota(tipo_pelota *p_pelota) {
	// Aleatorizamos la posicion inicial de la pelota
	p_pelota->x = rand() % NUM_COLUMNAS_DISPLAY;
	p_pelota->y = 2 + rand() % (NUM_FILAS_DISPLAY-2); // 2 evita que aparezca encima de ladrillos y para que no empiece demasiado pegada al suelo de la pantalla

	// Pelota inicialmente en el centro de la pantalla
	//p_pelota->x = NUM_COLUMNAS_DISPLAY/2 - 1;
	//p_pelota->y = NUM_FILAS_DISPLAY/2 -1 ;

	InicializaPosiblesTrayectorias(p_pelota);

	// Trayectoria inicial
	//p_pelota->trayectoria.xv = 0;
	//p_pelota->trayectoria.yv = 1;
	CambiarDireccionPelota(p_pelota, rand() % p_pelota->num_posibles_trayectorias);
}

void InicializaPala(tipo_pala *p_pala) {
	// Pala inicialmente en el centro de la pantalla
	p_pala->x = NUM_COLUMNAS_DISPLAY/2 - p_pala->ancho/2;
	p_pala->y = NUM_FILAS_DISPLAY - 1;
	p_pala->ancho = NUM_COLUMNAS_PALA;
	p_pala->alto = NUM_FILAS_PALA;
}

void InicializaPosiblesTrayectorias(tipo_pelota *p_pelota) {
	p_pelota->num_posibles_trayectorias = 0;
	p_pelota->posibles_trayectorias[ARRIBA_IZQUIERDA].xv = -1;
	p_pelota->posibles_trayectorias[ARRIBA_IZQUIERDA].yv = -1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ARRIBA].xv = 0;
	p_pelota->posibles_trayectorias[ARRIBA].yv = -1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ARRIBA_DERECHA].xv = 1;
	p_pelota->posibles_trayectorias[ARRIBA_DERECHA].yv = -1;
	p_pelota->num_posibles_trayectorias++;

	p_pelota->posibles_trayectorias[ABAJO_DERECHA].xv = 1;
	p_pelota->posibles_trayectorias[ABAJO_DERECHA].yv = 1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ABAJO].xv = 0;
	p_pelota->posibles_trayectorias[ABAJO].yv = 1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ABAJO_IZQUIERDA].xv = -1;
	p_pelota->posibles_trayectorias[ABAJO_IZQUIERDA].yv = 1;
	p_pelota->num_posibles_trayectorias++;

	//p_pelota->posibles_trayectorias[IZQUIERDA].xv = -1;
	//p_pelota->posibles_trayectorias[IZQUIERDA].yv = 0;
	//p_pelota->num_posibles_trayectorias++;
	//p_pelota->posibles_trayectorias[DERECHA].xv = 1;
	//p_pelota->posibles_trayectorias[DERECHA].yv = 0;
	//p_pelota->num_posibles_trayectorias++;
}

void PintaLadrillos(tipo_pantalla *p_ladrillos, tipo_pantalla *p_pantalla) {
	int i=0, j=0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_pantalla->matriz[i][j] = p_ladrillos->matriz[i][j];
		}
    }
}

void PintaPala(tipo_pala *p_pala, tipo_pantalla *p_pantalla) {
	int i=0, j=0;

	for(i=0;i<NUM_FILAS_PALA;i++) {
		for(j=0;j<NUM_COLUMNAS_PALA;j++) {
			if (( (p_pala->y+i >= 0) && (p_pala->y+i < NUM_FILAS_DISPLAY) ) &&
				( (p_pala->x+j >= 0) && (p_pala->x+j < NUM_COLUMNAS_DISPLAY) ))
				p_pantalla->matriz[p_pala->y+i][p_pala->x+j] = 1;
		}
	}
}

void PintaPelota(tipo_pelota *p_pelota, tipo_pantalla *p_pantalla) {
	if( (p_pelota->x >= 0) && (p_pelota->x < NUM_COLUMNAS_DISPLAY) ) {
		if( (p_pelota->y >= 0) && (p_pelota->y < NUM_FILAS_DISPLAY) ) {
			p_pantalla->matriz[p_pelota->y][p_pelota->x] = 8;
		}
		else {
			printf("\n\nPROBLEMAS!!!! posicion y=%d de la pelota INVALIDA!!!\n\n", p_pelota->y);
			fflush(stdout);
		}
	}
	else {
		printf("\n\nPROBLEMAS!!!! posicion x=%d de la pelota INVALIDA!!!\n\n", p_pelota->x);
		fflush(stdout);
	}
}

void ActualizaPantalla(tipo_arkanoPi* p_arkanoPi) {

    // Borro toda la pantalla
	ReseteaPantalla((tipo_pantalla*)(p_arkanoPi->p_pantalla));

    // Pinta los ladrillos
	PintaLadrillos(
		(tipo_pantalla*)(&(p_arkanoPi->ladrillos)),
		(tipo_pantalla*)(p_arkanoPi->p_pantalla));

    // Pinta la pala
	PintaPala(
		(tipo_pala*)(&(p_arkanoPi->pala)),
		(tipo_pantalla*)(p_arkanoPi->p_pantalla));

	// Pinta la pelota
	PintaPelota(
		(tipo_pelota*)(&(p_arkanoPi->pelota)),
		(tipo_pantalla*)(p_arkanoPi->p_pantalla));
}

void InicializaArkanoPi(tipo_arkanoPi *p_arkanoPi) {

	ResetArkanoPi(p_arkanoPi);
	piLock(MATRIX_KEY);
	ActualizaPantalla(p_arkanoPi);
	piUnlock(MATRIX_KEY);

	////MEJORA 6
	p_arkanoPi->vidas = 3;
}

void ResetArkanoPi(tipo_arkanoPi *p_arkanoPi) {
	ReseteaPantalla((tipo_pantalla*)(p_arkanoPi->p_pantalla));
	InicializaLadrillos((tipo_pantalla*)(&(p_arkanoPi->ladrillos)));
	InicializaPelota((tipo_pelota*)(&(p_arkanoPi->pelota)));
	InicializaPala((tipo_pala*)(&(p_arkanoPi->pala)));
}

void CambiarDireccionPelota(tipo_pelota *p_pelota, enum t_direccion direccion) {
	if((direccion < 0)||(direccion > p_pelota->num_posibles_trayectorias)) {
		printf("[ERROR!!!!][direccion NO VALIDA!!!!][%d]", direccion);
		return;
	}
	else {
		p_pelota->trayectoria.xv = p_pelota->posibles_trayectorias[direccion].xv;
		p_pelota->trayectoria.yv = p_pelota->posibles_trayectorias[direccion].yv;
	}
}

void ActualizaPosicionPala(tipo_pala *p_pala, enum t_direccion direccion) {
	switch (direccion) {
		case DERECHA:
			// Dejamos que la pala rebase parcialmente el lÃ­mite del area de juego
			if( p_pala->x + 1 + p_pala->ancho <= NUM_COLUMNAS_DISPLAY + 2 )
				p_pala->x = p_pala->x + 1;
			break;
		case IZQUIERDA:
			// Dejamos que la pala rebase parcialmente el lÃ­mite del area de juego
			if( p_pala->x - 1 >= -2)
					p_pala->x = p_pala->x - 1;
			break;
		default:
			printf("[ERROR!!!!][direccion NO VALIDA!!!!][%d]", direccion);
			break;
	}
}

void ActualizaPosicionPelota (tipo_pelota *p_pelota) {
	p_pelota->x += p_pelota->trayectoria.xv;
	p_pelota->y += p_pelota->trayectoria.yv;
}

int CompruebaReboteLadrillo (tipo_arkanoPi *p_arkanoPi) {
	int p_posible_ladrillo_x = 0;
	int p_posible_ladrillo_y = 0;

	p_posible_ladrillo_x = p_arkanoPi->pelota.x + p_arkanoPi->pelota.trayectoria.xv;

	if ( ( p_posible_ladrillo_x < 0) || ( p_posible_ladrillo_x >= NUM_COLUMNAS_DISPLAY ) ) {
		printf("\n\nERROR GRAVE!!! p_posible_ladrillo_x = %d!!!\n\n", p_posible_ladrillo_x);
		fflush(stdout);
		exit(-1);
	}

	p_posible_ladrillo_y = p_arkanoPi->pelota.y + p_arkanoPi->pelota.trayectoria.yv;

	if ( ( p_posible_ladrillo_y < 0) || ( p_posible_ladrillo_y >= NUM_FILAS_DISPLAY ) ) {
		printf("\n\nERROR GRAVE!!! p_posible_ladrillo_y = %d!!!\n\n", p_posible_ladrillo_y);
		fflush(stdout);
	}

	if(p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] > 0 ) {
		// La pelota ha entrado en el area de ladrillos
		// y descontamos el numero de golpes que resta para destruir el ladrillo
		p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] = p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] - 1;

		return 1;
	}
	return 0;
}

int CompruebaReboteParedesVerticales (tipo_arkanoPi arkanoPi) {
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if((arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv >= NUM_COLUMNAS_DISPLAY) ||
		(arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv < 0) ) {
		// La pelota rebota contra la pared derecha o izquierda
		return 1;
	}
	return 0;
}

int CompruebaReboteTecho (tipo_arkanoPi arkanoPi) {
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if(arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv < 0) {
		// La pelota rebota contra la pared derecha o izquierda
		return 1;
	}
	return 0;
}

int CompruebaRebotePala (tipo_arkanoPi arkanoPi) {
	if(arkanoPi.pelota.trayectoria.yv > 0) { // Esta condicion solo tiene sentido si la pelota va hacia abajo en la pantalla
		if ((arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv >= arkanoPi.pala.x ) &&
			(arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv < arkanoPi.pala.x + NUM_COLUMNAS_PALA)) {
				if ((arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv >= arkanoPi.pala.y) &&
					(arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv < arkanoPi.pala.y + NUM_FILAS_PALA)) {
					return 1;
				}
		}
	}
	return 0;
}

int CompruebaFallo (tipo_arkanoPi arkanoPi) {
	// Comprobamos si no hemos conseguido devolver la pelota
	if(arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv >= NUM_FILAS_DISPLAY) {
		// Hemos fallado, Activamos el flag de fallo
		piLock (SYSTEM_FLAGS_KEY);
		flags|= FLAG_FALLO;
		piUnlock (SYSTEM_FLAGS_KEY);
		return 1;
	}
	return 0;
}

int CalculaLadrillosRestantes(tipo_pantalla *p_ladrillos) {
	int i=0, j=0;
	int numLadrillosRestantes;

	numLadrillosRestantes = 0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			if(p_ladrillos->matriz[i][j] != 0) {
				numLadrillosRestantes++;
			}
		}
	}

	return numLadrillosRestantes;
}

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaBotonPulsado (fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_BOTON);
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaMovimientoIzquierda(fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_MOV_IZQUIERDA);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaMovimientoDerecha(fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_MOV_DERECHA);
	piUnlock (SYSTEM_FLAGS_KEY);


	return result;
}

int CompruebaTimeoutActualizacionJuego (fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_TIMER_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;

}

int CompruebaFinalJuego(fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_FIN_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

//------------------------------------------------------
/////MEJORA 6
int CompruebaHayVidas (fsm_t* this){
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_VIDA);
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaNoHayVidas (fsm_t* this){
	int result = 0;

		piLock (SYSTEM_FLAGS_KEY);
		result = (flags & FLAG_MUERTE);
		piUnlock (SYSTEM_FLAGS_KEY);
		return result;
}
int CompruebaFlagFallo (fsm_t* this){
	int result = 0;

		piLock (SYSTEM_FLAGS_KEY);
		result = (flags & FLAG_FALLO);
		piUnlock (SYSTEM_FLAGS_KEY);
		return result;
}
//------------------------------------------------------


//------------------------------------------------------
//MEJORA 4
int CompruebaPausa (fsm_t* this){
	int result = 0;

			piLock (SYSTEM_FLAGS_KEY);
			result = (flags & FLAG_PAUSA);
			piUnlock (SYSTEM_FLAGS_KEY);
			return result;
	}

int CompruebaReanuda (fsm_t* this){
	int result = 0;

			piLock (SYSTEM_FLAGS_KEY);
			result = (flags & FLAG_REANUDA);
			piUnlock (SYSTEM_FLAGS_KEY);
			return result;
	}
//------------------------------------------------------


//------------------------------------------------------
//MEJORA 5
int CompruebaLento(fsm_t* this){
	int result = 0;

			piLock (SYSTEM_FLAGS_KEY);
			result = (flags & FLAG_LENTO);
			piUnlock (SYSTEM_FLAGS_KEY);
			return result;
}
int CompruebaRapido(fsm_t* this){
	int result = 0;

			piLock (SYSTEM_FLAGS_KEY);
			result = (flags & FLAG_RAPIDO);
			piUnlock (SYSTEM_FLAGS_KEY);
			return result;
}
//------------------------------------------------------



//------------------------------------------------------
// FUNCIONES DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

// void InicializaJuego (void): funcion encargada de llevar a cabo
// la oportuna inicializaciÃ³n de toda variable o estructura de datos
// que resulte necesaria para el desarrollo del juego.

void InicializaJuego(fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno
	// ...

	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_BOTON);
	piUnlock (SYSTEM_FLAGS_KEY);

	InicializaArkanoPi(p_arkanoPi);
	tmr_startms(p_arkanoPi->tmr_actualizacion_juego, timeoutCool); //Si alguna vez queremos parar el timer, en lugar de TIMEOUT... se pone un 0
	tipo_pantalla *p_pantalla = p_arkanoPi->p_pantalla;

	//El timeoutCool de la línea 528 cambia en función de la elección del jugador y puede ser más rápido o más lento

	piLock (MATRIX_KEY);
	ActualizaPantalla(p_arkanoPi);
	piUnlock (MATRIX_KEY);

	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal (p_pantalla);
	piUnlock (STD_IO_BUFFER_KEY);

}

// void MuevePalaIzquierda (void): funcion encargada de ejecutar
// el movimiento hacia la izquierda contemplado para la pala.
// Debe garantizar la viabilidad del mismo mediante la comprobaciÃ³n
// de que la nueva posiciÃ³n correspondiente a la pala no suponga
// que Ã©sta rebase o exceda los lÃ­mites definidos para el Ã¡rea de juego
// (i.e. al menos uno de los leds que componen la raqueta debe permanecer
// visible durante todo el transcurso de la partida).

void MuevePalaIzquierda (fsm_t* this) {
	tipo_arkanoPi* p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_IZQUIERDA);
	flags &= (~FLAG_BOTON);
	piUnlock (SYSTEM_FLAGS_KEY);

	tipo_pantalla *p_pantalla = p_arkanoPi->p_pantalla;

	tipo_pala* p_pala = &p_arkanoPi->pala;

	//enum t_direccion direccion;
	ActualizaPosicionPala(p_pala, IZQUIERDA);

	piLock (MATRIX_KEY);
	ActualizaPantalla(p_arkanoPi);
	piUnlock (MATRIX_KEY);


	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal (p_pantalla);
	piUnlock (STD_IO_BUFFER_KEY);

}

// void MuevePalaDerecha (void): funciÃ³n similar a la anterior
// encargada del movimiento hacia la derecha.

void MuevePalaDerecha (fsm_t* this) {
	tipo_arkanoPi* p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_DERECHA);
	flags &= (~FLAG_BOTON);
	piUnlock (SYSTEM_FLAGS_KEY);

	tipo_pala* p_pala = &p_arkanoPi->pala;
	ActualizaPosicionPala(p_pala, DERECHA);

	piLock (MATRIX_KEY);
	ActualizaPantalla(p_arkanoPi);
	piUnlock (MATRIX_KEY);

	tipo_pantalla *p_pantalla = p_arkanoPi->p_pantalla;

	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal (p_pantalla);
	piUnlock (STD_IO_BUFFER_KEY);

}

// void ActualizarJuego (void): funciÃ³n encargada de actualizar la
// posiciÃ³n de la pelota conforme a la trayectoria definida para Ã©sta.
// Para ello deberÃ¡ identificar los posibles rebotes de la pelota para,
// en ese caso, modificar su correspondiente trayectoria (los rebotes
// detectados contra alguno de los ladrillos implicarÃ¡n adicionalmente
// la eliminaciÃ³n del ladrillo). Del mismo modo, deberÃ¡ tambiÃ©n
// identificar las situaciones en las que se dÃ© por finalizada la partida:
// bien porque el jugador no consiga devolver la pelota, y por tanto Ã©sta
// rebase el lÃ­mite inferior del Ã¡rea de juego, bien porque se agoten
// los ladrillos visibles en el Ã¡rea de juego.

void ActualizarJuego (fsm_t* this) {
	tipo_arkanoPi* p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

// para pasar de direccion a el valor de Ã©sta-> *p_a (poner el * delante del puntero)
//	int a = 3;
//	int* p_a;
//	p_a = &a;
//	int b = *p_a;
	// TiPO LOCAL -> NO VALE tipo_arkanoPi arkanoPi;

	// A completar por el alumno
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_TIMER_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);

	tipo_pelota* p_pelota = &p_arkanoPi->pelota;
	tipo_pantalla *p_pantalla = p_arkanoPi->p_pantalla;
	tmr_startms(p_arkanoPi->tmr_actualizacion_juego, timeoutCool);

	if(CompruebaReboteParedesVerticales(*p_arkanoPi)) {
		//p_arkanoPi->pelota.trayectoria.xv = -p_arkanoPi->pelota.trayectoria.xv;

		/////MEJORA 0 (para que el juego funcione correctamente sin errores)
		//Que en lugar de rebotar aparezca por la pared contraria
		//La trayectoria de mantiene y ahora lo que cambia es la posición de la pelotita
		//Intentamos cambiar primero la x
		//Si rebota en la pared izquierda es una cosa y si rebota en la derecha es otra :)
		if(p_arkanoPi->pelota.trayectoria.xv>0){
			p_arkanoPi->pelota.x=-1;
		}else if (p_arkanoPi->pelota.trayectoria.xv<0){
			p_arkanoPi->pelota.x=8;
		}
	}
	if(CompruebaReboteTecho(*p_arkanoPi)) {
		p_arkanoPi->pelota.trayectoria.yv= -p_arkanoPi->pelota.trayectoria.yv;
	}
	if(CompruebaFallo(*(p_arkanoPi))) {
		//Si fallamos se activa el flag_fin_juego
		piLock(SYSTEM_FLAGS_KEY);
		flags |= FLAG_FIN_JUEGO;
		piUnlock(SYSTEM_FLAGS_KEY);
		return;

	} else if (CompruebaRebotePala(*p_arkanoPi)) {
		switch(p_arkanoPi->pelota.x + p_arkanoPi->pelota.trayectoria.xv - p_arkanoPi->pala.x) {
		case 0:
			//MEJORA 0 (para que el juego funcione correctamente sin errores)
			if(p_arkanoPi->pelota.x == 0 && p_arkanoPi->pelota.y == 5){
				CambiarDireccionPelota(p_pelota, ARRIBA_DERECHA);
			}else{
			CambiarDireccionPelota(p_pelota,ARRIBA_IZQUIERDA);
			}
			break;
		case 1:
			CambiarDireccionPelota(p_pelota, ARRIBA);
			break;
		case 2:
			//MEJORA  0 (para que el juego funcione correctamente sin errores)
			if(p_arkanoPi->pelota.x == 7 && p_arkanoPi->pelota.y == 5){
				CambiarDireccionPelota(p_pelota, ARRIBA_IZQUIERDA);
			}else{
			CambiarDireccionPelota(p_pelota, ARRIBA_DERECHA);
			}
		}
	}
	tipo_pantalla* p_ladrillos = &p_arkanoPi->ladrillos;

	if (CompruebaReboteLadrillo(p_arkanoPi)) {
		p_arkanoPi->pelota.trayectoria.yv= -p_arkanoPi->pelota.trayectoria.yv;

		if(CalculaLadrillosRestantes(p_ladrillos)<= 0) {
			piLock(SYSTEM_FLAGS_KEY);
			flags |= FLAG_FIN_JUEGO;
			piUnlock(SYSTEM_FLAGS_KEY);
			return;
		}else if(CompruebaReboteTecho(*p_arkanoPi)){
			p_arkanoPi->pelota.trayectoria.xv = -p_arkanoPi->pelota.trayectoria.xv;
			p_arkanoPi->pelota.trayectoria.yv = -p_arkanoPi->pelota.trayectoria.yv;
		}
	}

	piLock (MATRIX_KEY);
	ActualizaPosicionPelota(p_pelota);
	ActualizaPantalla(p_arkanoPi);
	piUnlock (MATRIX_KEY);
	tmr_startms(p_arkanoPi->tmr_actualizacion_juego, timeoutCool);

	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal (p_pantalla);
	piUnlock (STD_IO_BUFFER_KEY);


}

// void FinalJuego (void): funciÃ³n encargada de mostrar en la ventana de
// terminal los mensajes necesarios para informar acerca del resultado del juego.

void FinalJuego (fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);
	// A completar por el alumno
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_FIN_JUEGO);
	flags &= (~FLAG_TIMER_JUEGO);
	flags &= (~FLAG_BOTON);
	piUnlock (SYSTEM_FLAGS_KEY);
	tmr_startms(p_arkanoPi->tmr_actualizacion_juego, 0);
	tipo_pantalla *p_ladrillos = &(p_arkanoPi->ladrillos);

	//////MEJORAS 2 y 3
	//------------------------------------------------------
	if((CalculaLadrillosRestantes(p_ladrillos)) == 0){

		piLock (MATRIX_KEY);
		PintaMensajeInicialPantalla(p_arkanoPi->p_pantalla, &pantalla_inicial );
		piUnlock (MATRIX_KEY);
		printf("\nHas ganadoooo wiiiiiiiiiiiiiiii BD\n");

		piLock (STD_IO_BUFFER_KEY);
		PintaPantallaPorTerminal (p_arkanoPi->p_pantalla);
		piUnlock (STD_IO_BUFFER_KEY);

	}else if ((CalculaLadrillosRestantes(p_ladrillos))>0){
		piLock (MATRIX_KEY);
		PintaMensajeInicialPantalla(p_arkanoPi->p_pantalla, &pantalla_final );
		piUnlock (MATRIX_KEY);

		printf("\nHas fallado :(\n");

		piLock (STD_IO_BUFFER_KEY);
		PintaPantallaPorTerminal (p_arkanoPi->p_pantalla);
		piUnlock (STD_IO_BUFFER_KEY);
	}
	//------------------------------------------------------
}

//void ReseteaJuego (void): funciÃ³n encargada de llevar a cabo la
// reinicializaciÃ³n de cuantas variables o estructuras resulten
// necesarias para dar comienzo a una nueva partida.

void ReseteaJuego (fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	flags =0; //Como hay muchos flags activados los limpiamos todos
	piUnlock (SYSTEM_FLAGS_KEY);

	piLock (MATRIX_KEY);
	ResetArkanoPi(p_arkanoPi);
	piUnlock (MATRIX_KEY);

	tipo_pantalla *p_pantalla = p_arkanoPi->p_pantalla;

	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal (p_pantalla);
	piUnlock (STD_IO_BUFFER_KEY);

}


//------------------------------------------------------
////MEJORA 6
//Cuando se comete un fallo saltamos a esta rutina en la que comprobamos el numero de vidas que hay
//Si es mayor a 0, disminuimos el número de vidas y activamos el flag_vida
//Si no, activamos el flag_muerte
//Además, cuando perdemos una vida, dibujamos en el display el número de vidas que quedan
void RestaVida (fsm_t* this){
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_VIDA);
	flags &= (~FLAG_MUERTE);
	flags &= (~FLAG_FALLO);
	piUnlock (SYSTEM_FLAGS_KEY);

	if(CompruebaFallo(*p_arkanoPi)){
		if(p_arkanoPi->vidas > 0){
			p_arkanoPi->vidas--;
			piLock(SYSTEM_FLAGS_KEY);
			flags |= FLAG_VIDA;
			piUnlock(SYSTEM_FLAGS_KEY);
		}else{
			piLock(SYSTEM_FLAGS_KEY);
			flags |= FLAG_MUERTE;
			piUnlock(SYSTEM_FLAGS_KEY);
		}
		switch(p_arkanoPi->vidas){
				case 2:
					piLock (MATRIX_KEY);
					PintaMensajeInicialPantalla(p_arkanoPi->p_pantalla, &dos );
					piUnlock (MATRIX_KEY);
					piLock (STD_IO_BUFFER_KEY);
					PintaPantallaPorTerminal (p_arkanoPi->p_pantalla);
					piUnlock (STD_IO_BUFFER_KEY);
					printf("\nTe quedan 2 vidas!\n");
					break;

				case 1:
					piLock (MATRIX_KEY);
					PintaMensajeInicialPantalla(p_arkanoPi->p_pantalla, &uno );
					piUnlock (MATRIX_KEY);
					piLock (STD_IO_BUFFER_KEY);
					PintaPantallaPorTerminal (p_arkanoPi->p_pantalla);
					piUnlock (STD_IO_BUFFER_KEY);
					printf("\nTe queda 1 vida!\n");
					break;

				case 0:
					piLock (MATRIX_KEY);
					PintaMensajeInicialPantalla(p_arkanoPi->p_pantalla, &cero );
					piUnlock (MATRIX_KEY);
					piLock (STD_IO_BUFFER_KEY);
					PintaPantallaPorTerminal (p_arkanoPi->p_pantalla);
					piUnlock (STD_IO_BUFFER_KEY);
					printf("\nTe has quedado sin vidas!\n");
					break;

				}
	}
}

//Para poder mostrar por pantalla el número de vidas y que se vea el tiempo suficiente como para que
//el usuario pueda visualizarlo usamos esta función, que deja durante un segundo el número en pantalla
void EsperaUnPoquito (fsm_t* this){
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_TIMER_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);

	tmr_startms(p_arkanoPi->tmr_actualizacion_juego, 1000);

}

// Tras perdida de 1 vida el juego continua donde se habia quedado inicializando la pelota en un
// sitio random, con inicializaPelota y la pala también con inicializaPala
void ContinuaJuego (fsm_t* this){
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);
	tipo_pantalla *p_pantalla = p_arkanoPi->p_pantalla;

	piLock (SYSTEM_FLAGS_KEY);
	flags=0; //LIMPIAMOS TODOS LOS FLAGS porque hay muchos activados
	piUnlock (SYSTEM_FLAGS_KEY);

	tmr_startms(p_arkanoPi->tmr_actualizacion_juego, timeoutCool);

	InicializaPelota(&(p_arkanoPi->pelota));
	InicializaPala(&(p_arkanoPi->pala));
	piLock(MATRIX_KEY);
	ActualizaPantalla(p_arkanoPi);
	piUnlock(MATRIX_KEY);

	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal (p_pantalla);
	piUnlock (STD_IO_BUFFER_KEY);

}

//------------------------------------------------------
///////MEJORA 4
////Pausa el temporizador de actualización del juego para pausar el mismo
void Pausa (fsm_t* this){
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	piLock (SYSTEM_FLAGS_KEY); //Limpiamos flags
	flags &= (~FLAG_PAUSA);
	flags &= (~FLAG_REANUDA);
	flags &= (~FLAG_BOTON);
	piUnlock (SYSTEM_FLAGS_KEY);

	printf("\nHas pausado el juego\n");

	tmr_startms(p_arkanoPi->tmr_actualizacion_juego, 0);

}

////Reactiva el temporizador de actualización del juego para reanudar el mismo
void Play (fsm_t* this){
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	piLock (SYSTEM_FLAGS_KEY); //Limpiamos los flags
	flags &= (~FLAG_PAUSA);
	flags &= (~FLAG_REANUDA);
	flags &= (~FLAG_BOTON);
	piUnlock (SYSTEM_FLAGS_KEY);

	printf("\nHas reanudado el juego\n");
	tmr_startms(p_arkanoPi->tmr_actualizacion_juego, timeoutCool);
}
//------------------------------------------------------


//------------------------------------------------------
////////MEJORA 5
void JuegoRapido (fsm_t* this){
	timeoutCool = TIMEOUT_ACTUALIZA_JUEGO_RAPIDO;
	InicializaJuego (this);
}
void JuegoLento (fsm_t* this){
	timeoutCool = TIMEOUT_ACTUALIZA_JUEGO;
	InicializaJuego (this);
}
//------------------------------------------------------


//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void tmr_actualizacion_juego_isr(union sigval value) {
	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags|= FLAG_TIMER_JUEGO;
	piUnlock(SYSTEM_FLAGS_KEY);
}


