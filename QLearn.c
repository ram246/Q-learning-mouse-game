/*
	CSC D84 - Unit 3 - Reinforcement Learning

	This file contains stubs for implementing the Q-Learning method
	for reinforcement learning as discussed in lecture. You have to
	complete two versions of Q-Learning.

	* Standard Q-Learning, based on a full-state representation and
	  a large Q-Table
	* Feature based Q-Learning to handle problems too big to allow
	  for a full-state representation

	Read the assignment handout carefully, then implement the
	required functions below. Sections where you have to add code
	are marked

	**************
	*** TO DO:
	**************

	If you add any helper functions, make sure you document them
	properly and indicate in the report.txt file what you added.

	Have fun!

	DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
	management being done properly, and for memory leaks.

	Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"
int DikstrasCheese[5][32][32];
int CALL_A_STAR = 1;

int oldCheesePos[1][2];
int prevMoves[10][2];
int prevMoveIndex = 0;


void QLearn_update(int s, int a, double r, int s_new, double *QTable)
{
 /*
   This function implementes the Q-Learning update as stated in Lecture. It
   receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.

   Your work here is to calculate the required update for the Q-table entry
   for state s, and apply it to the Q-table

   The update involves two constants, alpha and lambda, which are defined in QLearn.h - you should not
   have to change their values. Use them as they are.

   Details on how states are used for indexing into the QTable are shown
   below, in the comments for QLearn_action. Be sure to read those as well!
 */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
    int max_qsa_new = *(QTable+(4 * s_new) + 0);
    for (int i = 1; i < 4; i ++){
      int temp = *(QTable+(4 * s_new) + i);
      if (temp > max_qsa_new){
        max_qsa_new = temp;
      }
    }
  *(QTable+(4 * s) + a)  += alpha * (r + (lambda * max_qsa_new) - *(QTable+(4 * s) + a));
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, double *QTable, int size_X, int graph_size)
{
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through walls
     - The mouse position
     - The cat position
     - The chees position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the QTable to decide its action,
       for example, if pct=.25, then 25% of the time the mouse uses the QTable to choose its action,
       the remaining 75% of the time it chooses randomly among the available actions.

     Remember that the training process involves random exploration initially, but as training
     proceeds we use the QTable more and more, in order to improve our QTable values around promising
     actions.

     The value of pct is controlled by QLearn_core_GL, and increases with each round of training.

     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left

     QLearn_core_GL will print a warning if your action makes the mouse cross a wall, or if it makes
     the mouse leave the map - this should not happen. If you see a warning, fix the code in this
     function!

   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of

        graph_size^3 x 4

   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese.
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.

   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall.

   So all in all, you have a big table.

   For example, on an 8x8 maze, the Q-table will have a size of

       64^3 x 4  entries

       with

       size_X = 8		<--- size of one side of the maze
       graph_size = 64		<--- Total number of nodes in the graph

   Indexing within the Q-table works as follows:

     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n

     state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) + ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!

     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]

     (yes, it's a linear array, no shorcuts with brackets!)

     NOTE: There is only one cat and once cheese, so you only need to use cats[0][:] and cheeses[0][:]
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  double random = drand48() ;
  int state = encodeState(mouse_pos, cats, cheeses, size_X, graph_size);
  int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1] * size_X);
  if (random <= pct){
    int max_action = 0;
    int init = 1;
    int max_qsa_new = 0;
    for (int i = 0; i < 4; i ++){
      int temp = *(QTable+(4 * state) + i);
      if (gr[mouse_index][i] && (init || temp > max_qsa_new)){
        max_qsa_new = temp;
        max_action = i;
        init = 0;
      }
    }
    return max_action;
  } else {
    while (1){
      double r = drand48();
      int action;
      if (r <= 0.25){
        action = 0;
      } else if (r <= 0.5){
        action = 1;
      } else if (r <= 0.75){
        action = 2;
      } else {
        action = 3;
      }
      if (gr[mouse_index][action]){
        return action;
      }
    }
  }
  return(0);

}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function computes and returns a reward for the state represented by the input mouse, cat, and
    cheese position.

    You can make this function as simple or as complex as you like. But it should return positive values
    for states that are favorable to the mouse, and negative values for states that are bad for the
    mouse.

    I am providing you with the graph, in case you want to do some processing on the maze in order to
    decide the reward.

    This function should return a maximim/minimum reward when the mouse eats/gets eaten respectively.
   */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
   // Check for cats having lunch
  for (int i=0; i<5 && cats[i][0] != -1; i++)
    if (mouse_pos[0][0]==cats[i][0]&&mouse_pos[0][1]==cats[i][1]) return(-50000);

 // Check for mouse having lunch
  for (int i=0; i<5 && cheeses[i][0] != -1; i++)
    if (mouse_pos[0][0]==cheeses[i][0]&&mouse_pos[0][1]==cheeses[i][1]) return(50000);
  // We want to punish mouse for not getting cheese fast
  return(-50);
}

void feat_QLearn_update(double gr[max_graph_size][4],double weights[25], double reward, int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
    This function performs the Q-learning adjustment to all the weights associated with your
    features. Unlike standard Q-learning, you don't receive a <s,a,r,s'> tuple, instead,
    you receive the current state (mouse, cats, and cheese potisions), and the reward
    associated with this action (this is called immediately after the mouse makes a move,
    so implicit in this is the mouse having selected some action)

    Your code must then evaluate the update and apply it to the weights in the weight array.
   */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
    double maxU;
    int maxA;
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &maxA);
    double features[maxFeatures];
    evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
    double qsa = Qsa(weights, features);
    for (int i = 0; i < numFeatures; i++){
        weights[i] += (alpha * (reward + (lambda * maxU) - qsa)) * features[i];
    }

		// for (int j = 0; j < numFeatures; j++){
		// 		printf("weight is: %f\n", weights[j]);
		// }
}

int feat_QLearn_action(double gr[max_graph_size][4],double weights[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], double pct, int size_X, int graph_size)
{
  /*
    Similar to its counterpart for standard Q-learning, this function returns the index of the next
    action to be taken by the mouse.

    Once more, the 'pct' value controls the percent of time that the function chooses an optimal
    action given the current policy.

    E.g. if 'pct' is .15, then 15% of the time the function uses the current weights and chooses
    the optimal action. The remaining 85% of the time, a random action is chosen.

    As before, the mouse must never select an action that causes it to walk through walls or leave
    the maze.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  double random = drand48() ;
  int maxA;
  int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1] * size_X);
  if (random <= pct){
        double maxU;
        maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU, &maxA);

  } else {
    while (1){
      double r = drand48();
      int action;
      if (r <= 0.25){
        action = 0;
      } else if (r <= 0.5){
        action = 1;
      } else if (r <= 0.75){
        action = 2;
      } else {
        action = 3;
      }
      if (gr[mouse_index][action]){
        maxA = action;
        break;
      }
    }
  }
  int new_x;
  int new_y;
  if (maxA == 0){
    new_x = mouse_pos[0][0];
    new_y = mouse_pos[0][1] - 1;
  }
  // Check if right can be reached
  else if (maxA== 1){
    new_x = mouse_pos[0][0] + 1;
    new_y = mouse_pos[0][1];
  }
  // Check is down can be reached
  else if (maxA == 2){
    new_x = mouse_pos[0][0];
    new_y = mouse_pos[0][1] + 1;
  }
  // Check if left can be reached
  else{
    new_x = mouse_pos[0][0] - 1;
    new_y = mouse_pos[0][1];
  }
  int replace = prevMoveIndex;
  prevMoves[replace][0] = new_x;
  prevMoves[replace][1] = new_y;
  if (replace == 9){
    prevMoveIndex = 0;
  } else {
    prevMoveIndex += 1;
  }

  return(maxA);

}

void evaluateFeatures(double gr[max_graph_size][4],double features[25], int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size)
{
  /*
   This function evaluates all the features you defined for the game configuration given by the input
   mouse, cats, and cheese positions. You are free to define up to 25 features. This function will
   evaluate each, and return all the feature values in the features[] array.

   Take some time to think about what features would be useful to have, the better your features, the
   smarter your mouse!

   Note that instead of passing down the number of cats and the number of cheese chunks (too many parms!)
   the arrays themselves will tell you what are valid cat/cheese locations.

   You can have up to 5 cats and up to 5 cheese chunks, and array entries for the remaining cats/cheese
   will have a value of -1 - check this when evaluating your features!
   feature[0] = Minimum distance from mouse to cat (manhattan)
   feature[1] = Minimum distance from mouse to cheese (manhattan)
  */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  	if (CALL_A_STAR || oldCheesePos[0][1] != cheeses[0][1] || oldCheesePos[0][0] != cheeses[0][0]) {
      for (int i = 0; i < 10 ; i++){
        prevMoves[i][0]= -1;
        prevMoves[i][1] = -1;
      }
      for(int k = 0; k < maxObjects && cheeses[k][0] != -1; k++){
        for (int i = 0; i < size_X; i++){
          for (int j = 0; j < size_X; j++){
            DikstrasCheese[k][i][j] = 0;
          }
        }
        int single_cheese[1][2] = {cheeses[k][0], cheeses[k][1]};
        Dikstras(gr, size_X, single_cheese, k);
        oldCheesePos[0][0] = cheeses[0][0];
        oldCheesePos[0][1] = cheeses[0][1];
      }
      CALL_A_STAR = 0;
		}
    int min_cat_dist = abs(mouse_pos[0][0] - cats[0][0]) + abs(mouse_pos[0][1] - cats[0][1]);
    double min_cat_eclid = pow(pow(mouse_pos[0][0] - cats[0][0], 2) + pow(mouse_pos[0][1] - cats[0][1], 2), 0.5);
    int cat_dist;
    for (int i = 1; i < maxObjects && cats[i][0] != -1; i++){
        cat_dist = abs(mouse_pos[0][0] - cats[i][0]) + abs(mouse_pos[0][1] - cats[i][1]);
        if (cat_dist < min_cat_dist){
          min_cat_dist = cat_dist;
        }
        double cat_eclid = pow(pow(mouse_pos[0][0] - cats[i][0], 2) + pow(mouse_pos[0][1] - cats[i][1], 2), 0.5);

        if (cat_eclid < min_cat_eclid){
          min_cat_eclid = cat_eclid;
        }
    }

    int min_cheese_dist = abs(mouse_pos[0][0] - cheeses[0][0]) + abs(mouse_pos[0][1] - cheeses[0][1]);
    double min_cheese_eclid = pow(pow(mouse_pos[0][0] - cheeses[0][0], 2) + pow(mouse_pos[0][1] - cheeses[0][1], 2), 0.5);
		int min_cheese_id = 0;
    int cheese_dist;
    for (int i = 1; i < maxObjects && cheeses[i][0] != -1; i++){
        cheese_dist = abs(mouse_pos[0][0] - cheeses[i][0]) + abs(cheeses[0][1] - cheeses[i][1]);
        if (cheese_dist < min_cheese_dist){
          min_cheese_dist = cheese_dist;
					min_cheese_id = i;
        }
        double cheese_eclid = pow(pow(mouse_pos[0][0] - cheeses[i][0], 2) + pow(mouse_pos[0][1] - cheeses[i][1], 2), 0.5);

        if (cheese_eclid < min_cheese_eclid){
          min_cheese_eclid = cheese_eclid;
        }
    }

    int min_cheese_shortest_dist = DikstrasCheese[0][mouse_pos[0][0]][mouse_pos[0][1]];
    for (int i = 1; i < maxObjects && cheeses[i][0] != -1; i++){
        cheese_dist = DikstrasCheese[i][mouse_pos[0][0]][mouse_pos[0][1]];
        if (cheese_dist < min_cheese_shortest_dist){
          min_cheese_shortest_dist = cheese_dist;
					min_cheese_id = i;

        }
    }
		int numWalls = 0;
		int mouse_index = mouse_pos[0][0] + (mouse_pos[0][1] * size_X);
		for (int i = 0; i < 4; i++){
			if (!gr[mouse_index][i]){
				numWalls++;
			}
		}
    int prev = 0;

    for (int i = 0; i < 10 && prevMoves[i][0] != -1; i++){
      if (mouse_pos[0][0] == prevMoves[i][0] && mouse_pos[0][1] == prevMoves[i][1]){
        prev = 1;
        break;
      }
    }

    features[0] = 1 / (1+ min_cat_dist);
    features[1] = 1 / (1+min_cheese_shortest_dist);
    features[2] = 1 / (1+min_cheese_dist);
		features[3] = 1/(1+numWalls);
    features[4] = 1;
    features[5] = prev;
    features[6] = 1 / (1 + min_cheese_eclid);
    features[7] = 1 / (1 + min_cat_eclid);

    for (int i = numFeatures; i < maxFeatures; i++){
        features[i] = 0;
    }

}

double Qsa(double weights[25], double features[25])
{
    /*
    Compute and return the Qsa value given the input features and current weights
    */

    /***********************************************************************************************
    * TO DO: Complete this function
    ***********************************************************************************************/
    double qsa = 0;
    for (int i = 0; i < maxFeatures; i++){
        qsa += weights[i] * features[i];
    }

    return(qsa);		// <--- stub! compute and return the Qsa value
}

void maxQsa(double gr[max_graph_size][4],double weights[25],int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size, double *maxU, int *maxA)
{
 /*
   Given the state represented by the input positions for mouse, cats, and cheese, this function evaluates
   the Q-value at all possible neighbour states and returns the max. The maximum value is returned in maxU
   and the index of the action corresponding to this value is returned in maxA.

   You should make sure the function does not evaluate moves that would make the mouse walk through a
   wall.
  */

   /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/

  int init = 1;
	int outer_init = 1;
  int x = mouse_pos[0][0];
  int y = mouse_pos[0][1];
  int mouse_index = x + (y * size_X);
	int cat_copy[5][2];
	double qsa;
	for (int i = 0; i < 5; i++){
		cat_copy[i][0] = cats[i][0];
		cat_copy[i][1] = cats[i][1];
	}
  for (int i = 0; i < 4 ; i++){
      if (!gr[mouse_index][i]){
        continue;
      }
      int new_mouse_pos[1][2];
      int new_x;
      int new_y;
      if (i == 0){
        new_x = x;
        new_y = y - 1;
      }
      // Check if right can be reached
      else if (i== 1){
        new_x = x + 1;
        new_y = y;
      }
      // Check is down can be reached
      else if (i == 2){
        new_x = x;
        new_y = y + 1;
      }
      // Check if left can be reached
      else{
        new_x = x - 1;
        new_y = y;
      }
      new_mouse_pos[0][0] = new_x;
      new_mouse_pos[0][1] = new_y;
			//maxQsaHelper(gr, weights, new_mouse_pos, cat_copy, cheeses, size_X, graph_size, &qsa, 0, &init);
      double features[maxFeatures];
      evaluateFeatures(gr, features, new_mouse_pos, cats, cheeses, size_X, graph_size);
      qsa = Qsa(weights, features);
			if (outer_init || qsa > *maxU){
				*maxU = qsa;
				*maxA = i;
				outer_init = 0;
			}
  }
  return;

}

/***************************************************************************************************
 *  Add any functions needed to compute your features below
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/

int encodeState(int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X, int graph_size){

    double power = 1;
    // Add the mouse value first
    int state = mouse_pos[0][0] + (mouse_pos[0][1] * size_X);

    // Add the cats value
    for (int i = 0; i < maxObjects && cats[i][0] != -1; i++){
        state += (cats[i][0] + (cats[i][1] * size_X)) * pow(graph_size, power);
        power++;
    }

    // Add the cheeses value
    for (int i = 0; i < maxObjects && cheeses[i][0] != -1; i++){
        state += (cheeses[i][0] + (cheeses[i][1] * size_X)) * pow(graph_size, power);
        power++;
    }
    return (state);
}

void maxQsaHelper(double gr[max_graph_size][4],double weights[25],int mouse_pos[1][2], int cats[5][2], int cheeses[5][2],
									int size_X, int graph_size, double *maxU, int catId, int *init){
		if (catId < maxObjects && cats[catId][0] != -1){
			int cat_index = cats[catId][0] + (cats[catId][1] * size_X);
			int x = cats[catId][0];
			int y = cats[catId][1];
			int new_x;
			int new_y;
			for (int j = -1; j < 4; j++){
				if (!gr[cat_index][j]){
					continue;
				}
				if (j == -1){
					new_x = x;
					new_y = y;
				}
				else if (j == 0){
	        new_x = x;
	        new_y = y - 1;
	      }
	      // Check if right can be reached
	      else if (j== 1){
	        new_x = x + 1;
	        new_y = y;
	      }
	      // Check is down can be reached
	      else if (j == 2){
	        new_x = x;
	        new_y = y + 1;
	      }
	      // Check if left can be reached
	      else{
	        new_x = x - 1;
	        new_y = y;
	      }
				cats[catId][0] = new_x;
	      cats[catId][1] = new_y;
				maxQsaHelper(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, maxU, catId + 1, init);
			}
		} else {
				double features[maxFeatures];
				evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
				double qsa = Qsa(weights, features);
				if (*init || (*maxU < qsa)){
					*init = 0;
					*maxU = qsa;
				}
		}

}

/* Insert for priority queue
*/
void insert(struct PQNode head[max_graph_size], int point[2], int heuristic_val) {
  int pos;
  for (int i = 0; i < max_graph_size; i++) {
    if (head[i].priority == -1){
      pos = i;
      break;
    }
    if (heuristic_val >= head[i].priority ){
      pos = i;
			break;
    }
  }

  int end_index = max_graph_size - 1;
  while (end_index >=0 && head[end_index].priority == -1) end_index--;

  int j = end_index + 1;

  while (j != pos){
    head[j] = head[j - 1];
		j--;
  }

  head[j].point[0] = point[0];
  head[j].point[1] = point[1];
  head[j].priority = heuristic_val;
}

/* Modify for priority queue
*/
void modify(struct PQNode head[max_graph_size], int index, int new_val) {
	struct PQNode temp = head[index];
	for (int i = index; i < max_graph_size - 1; i++){
		head[i] = head[i + 1];
		if (head[i].priority == -1) break;
	}
	temp.priority = new_val;
	insert(head, temp.point, temp.priority);
}

/* Pop for priority queue
*/
void pop(struct PQNode head[max_graph_size], struct PQNode *ret) {
	for (int i = 0; i < max_graph_size; i++){
		if (head[i].priority == -1){
			if (i > 0){
				*ret = head[i-1];
				head[i-1].priority = -1;
			}
			break;
		}
	}
}

/* Check if priority queue is empty
*/
int is_empty(struct PQNode head[max_graph_size]){
	return head[0].priority == -1;
}

/* Finds element in priority queue
*/
int find(struct PQNode head[max_graph_size], int point[2]){
	for (int i = 0; i < max_graph_size; i++){
		if (head[i].point[0] == point[0] && head[i].point[1] == point[1] && head[i].point[0] != -1){
			return i;
		}
	}

	return -1;
}

double Dikstras(double gr[max_graph_size][4], int size_X, int agent[1][2], int graphIndex){
	// Counter for visited
	int visit_counter = 0;

	// Keep track of parent nodes
	// visited_list[graph_size][0] - 0 if not visited o/w 1
	// visited_list[graph_size][1] - heuristic cost it takes to get to this node
	// visited_list[graph_size][2] - index of parent node
	// visited_list[graph_size][3] - actual cost
	int visited_list[max_graph_size][4];

	// First create the head of the priority queue
	struct PQNode priority_queue[max_graph_size];
	int i;
	for (i = 0; i < max_graph_size; i++){
		priority_queue[i].priority = -1;
		priority_queue[i].point[0] = -1;
		priority_queue[i].point[1] = -1;
		visited_list[i][0] = 0;
		visited_list[i][1] = 0;
		visited_list[i][2] = -1;
		visited_list[i][3] = 0;
	}

	int mouse_index = agent[0][0] + (agent[0][1] * size_X);
	// Set priority to 0 to ensure it remains at start of queue
	insert(priority_queue, agent[0], 0);
	visited_list[mouse_index][2] = mouse_index;

	int cheese_found = 0;
		int goal_loc = -1;

	while (!is_empty(priority_queue) && !cheese_found){
		// Pop out the head
		struct PQNode current_node;
		pop(priority_queue, &current_node);
		int x = current_node.point[0];
		int y = current_node.point[1];
		int priority = current_node.priority;
		int index = x + (y * size_X);
		DikstrasCheese[graphIndex][x][y] = priority;

		// Mark node as visited
		visited_list[index][0] = 1;
		visited_list[index][1] = priority;


		for (int j = 0; j < 4; j++){
			// Check if top can be reached
			int new_x;
			int new_y;
			if (j == 0){
				new_x = x;
				new_y = y - 1;
			}
			// Check if right can be reached
			else if (j == 1){
				new_x = x + 1;
				new_y = y;
			}
			// Check is down can be reached
			else if (j == 2){
				new_x = x;
				new_y = y + 1;
			}
			// Check if left can be reached
			else{
				new_x = x - 1;
				new_y = y;
			}
			if (gr[index][j]){
				int new_point[2] = {new_x, new_y};
				int new_point_index = new_x + (( new_y) * size_X);
				int h = 0;
				int found_point = find(priority_queue, new_point);

				int total_cost = h + visited_list[index][3] + 1;

				// If point is not in queue yet place it in
				if (found_point == -1 && !visited_list[new_point_index][0]){
					insert(priority_queue, new_point, total_cost);
					visited_list[new_point_index][2] = index;
					visited_list[new_point_index][3] = visited_list[index][3] + 1;
				}
				// If point in queue but the priority decresed now then reduce it
				else if (priority_queue[found_point].priority > total_cost && !visited_list[new_point_index][0]){
					modify(priority_queue, found_point, total_cost);
					visited_list[new_point_index][2] = index;
					visited_list[new_point_index][3] = visited_list[index][3] + 1;
				}
			}
		}
  	}
	 return 1;
}
