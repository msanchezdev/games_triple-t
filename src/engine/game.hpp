#pragma once

/**
 * Function used to initialize game data.
 */
void Game_Init();

/**
 * Load all resources used by the game here.
 */
void Game_Resources();

/**
 * Main render function for the game.
 * All assets should be rendered here
 */
void Game_Render();

/**
 * Function used to cleanup game data.
 */
void Game_Cleanup();
