import numpy as np



class AIPlayer:
    def __init__(self, player_number):
        self.player_number = player_number
        #set player number and opponent number
        if self.player_number == 1:
            self.opp = 2
        else: 
            self.opp = 1
        self.type = 'ai'
        self.player_string = 'Player {}:ai'.format(player_number)

    #check for valid columns to drop pieces
    def valid_locations(self, board):
        valid_cols = []
        for i, col in enumerate(board.T):
            if col[0]==0:
                valid_cols.append(i)
        return valid_cols

    def next_row(self, board, col):
        #find next available row in colomn
        for r in range(5,-1,-1):
            if board[r][col]==0:
                return r  

    def fill(self, row, col, board, player):
        board[row][col]=player

    def get_alpha_beta_move(self, board):
        """
        Given the current state of the board, return the next move based on
        the alpha-beta pruning algorithm

        This will play against either itself or a human player

        INPUTS:
        board - a numpy array containing the state of the board using the
                following encoding:
                - the board maintains its same two dimensions
                    - row 0 is the top of the board and so is
                      the last row filled
                - spaces that are unoccupied are marked as 0
                - spaces that are occupied by player 1 have a 1 in them
                - spaces that are occupied by player 2 have a 2 in them

        RETURNS:
        The 0 based index of the column that represents the next move
        """
        zeros = np.count_nonzero(board==0)
        
        turn = 42 - zeros
        print(turn)
        print(zeros, max(turn//5,2))

        if(zeros==1):
            zeros +=1
       

        col, value = self.max_value(min(max(turn//5,2),zeros-1), board, -1000000000, 1000000000)
        #print("Final value:" , value)
        return col

    def max_value(self, depth, board, alpha, beta):

        valid_cols = self.valid_locations(board)
        if len(valid_cols):
            column = np.random.choice(valid_cols)

        #terminal = self.is_terminal(board) #game.IS-TERMINAL
        terminal, eval_score = self.evaluation_function(board)

        if terminal or depth==0: 
            return (None, eval_score)
            
        maxScore = -1000000000 # v <- (-infinity)

        for col in valid_cols:
            row = self.next_row(board, col)
            board_copy = board.copy()
            self.fill(row, col, board_copy, self.player_number)
            score = self.min_value(depth-1, board_copy, alpha, beta)[1]

            #select col with highest score
            if score > maxScore:
                maxScore = score
                column = col
                alpha = max(alpha, maxScore)

            ##pruning
            if beta <= alpha:
                return column, maxScore

        return column, maxScore

    def min_value(self, depth, board, alpha, beta):

        valid_cols = self.valid_locations(board)
        if len(valid_cols):
            column = np.random.choice(valid_cols)

        #terminal = self.is_terminal(board) #game.IS-TERMINAL
        terminal, eval_score = self.evaluation_function(board)

        if terminal or depth==0: 
            return (None, eval_score)
            

        maxScore = 1000000000 # v <- (-infinity)

        for col in valid_cols:
            row = self.next_row(board, col)
            board_copy = board.copy()
            self.fill(row, col, board_copy, self.opp)
            #print(board_copy)
            score = self.max_value(depth-1, board_copy, alpha, beta)[1]
            #print(score)

            if score < maxScore:
                maxScore = score
                column = col
                beta = min(beta, maxScore)

            #pruning
            if beta <= alpha:
                #print("return" ,maxScore)
                return column, maxScore

        #print("return" ,maxScore)
        return column, maxScore

    def exp_max_value(self, depth, board, alpha, beta):
        valid_cols = self.valid_locations(board)
        column = np.random.choice(valid_cols)

        terminal = self.is_terminal(board) #game.IS-TERMINAL
        if terminal or depth==0: 
            return (None, self.evaluation_function(board))
            

        value = -1000000000 # v <- (-infinity)

        for col in valid_cols:
            row = self.next_row(board, col)
            board_copy = board.copy()
            self.fill(row, col, board_copy, self.player_number)
            score = self.min_value(depth-1,board_copy, alpha, beta)[1]
            if score > value:
                value = score
                column = col
            alpha = max(alpha, value)
            if beta <= alpha:
                return column, value

        return column, value

    #def exp_value(self, depth, board, alpha, beta):
        valid_cols = self.valid_locations(board)
        column = np.random.choice(valid_cols)

        terminal = self.is_terminal(board) #game.IS-TERMINAL
        if terminal or depth==0: 
            return (None, self.evaluation_function(board))
            

        value = 0 

        for col in valid_cols:
            row = self.next_row(board, col)
            board_copy = board.copy()
            self.fill(row, col, board_copy, self.player_number)
            score = self.exp_max_value(depth-1, board_copy, alpha, beta)[1]
            if score <= value:
                value = score
                column = col
            beta = value/len(valid_cols)
            if alpha >= beta:
                break
        return column, value

    def get_expectimax_move(self, board):
        """
        Given the current state of the board, return the next move based on
        the expectimax algorithm.

        This will play against the random player, who chooses any valid move
        with equal probability

        INPUTS:
        board - a numpy array containing the state of the board using the
                following encoding:
                - the board maintains its same two dimensions
                    - row 0 is the top of the board and so is
                      the last row filled
                - spaces that are unoccupied are marked as 0
                - spaces that are occupied by player 1 have a 1 in them
                - spaces that are occupied by player 2 have a 2 in them

        RETURNS:
        The 0 based index of the column that represents the next move
        """
        col, value = self.exp_max_value(2, board, -1000000000, 1000000000)
        #raise NotImplementedError('Whoops I don\'t know what to do')
        return col

    def evaluate_section(self, section):

        #find opponents player number
        if self.player_number == 1:
            opp = 2
        else: 
            opp = 1

        game_end = False
        score = 0

        #evalueate different sections
        if section.count(self.player_number) == 4:
            score += 10000
            game_end = True
        elif section.count(opp) == 4:
            score -= 10000
            game_end = True
        elif section.count(self.player_number) == 3 and section.count(0) == 1:
            score += 1000
        elif section.count(self.player_number) == 2 and section.count(0) == 2:
            score += 100

        if section.count(opp) == 3 and section.count(0) == 1:
            score -= 1000

        return game_end, score

    def evaluation_function(self, board):
        """
        Given the current stat of the board, return the scalar value that 
        represents the evaluation function for the current player
       
        INPUTS:
        board - a numpy array containing the state of the board using the
                following encoding:
                - the board maintains its same two dimensions
                    - row 0 is the top of the board and so is
                      the last row filled
                - spaces that are unoccupied are marked as 0
                - spaces that are occupied by player 1 have a 1 in them
                - spaces that are occupied by player 2 have a 2 in them

        RETURNS:
        The utility value for the current board
        """
        score = 0
        is_terminal = False
        
        ## Score Horizontal
        for r in range(6):
            row_array = [int(i) for i in list(board[r,:])]
            for c in range(7-3):
                section = row_array[c:c+4]
                game_end, eval_score = self.evaluate_section(section)
                score += eval_score
                is_terminal = is_terminal or game_end

        ## Score Vertical
        for c in range(7):
            col_array = [int(i) for i in list(board[:,c])]
            for r in range(6-3):
                section = col_array[r:r+4]
                game_end, eval_score = self.evaluate_section(section)
                score += eval_score
                is_terminal = is_terminal or game_end

        for r in range(6-3):
            for c in range(7-3):
                ## Score posiive sloped diagonal
                section = [board[r+i][c+i] for i in range(4)]
                game_end, eval_score = self.evaluate_section(section)
                score += eval_score
                is_terminal = is_terminal or game_end

                ## Score negative sloped diagonal
                section = [board[r+3-j][c+j] for j in range(4)]
                game_end, eval_score = self.evaluate_section(section)
                score += eval_score
                is_terminal = is_terminal or game_end

        is_terminal = is_terminal or len(self.valid_locations(board)) == 0

        return is_terminal, score


class RandomPlayer:
    def __init__(self, player_number):
        self.player_number = player_number
        self.type = 'random'
        self.player_string = 'Player {}:random'.format(player_number)

    def get_move(self, board):
        """
        Given the current board state select a random column from the available
        valid moves.

        INPUTS:
        board - a numpy array containing the state of the board using the
                following encoding:
                - the board maintains its same two dimensions
                    - row 0 is the top of the board and so is
                      the last row filled
                - spaces that are unoccupied are marked as 0
                - spaces that are occupied by player 1 have a 1 in them
                - spaces that are occupied by player 2 have a 2 in them

        RETURNS:
        The 0 based index of the column that represents the next move
        """
        valid_cols = []
        for col in range(board.shape[1]):
            if 0 in board[:,col]:
                valid_cols.append(col)

        return np.random.choice(valid_cols)


class HumanPlayer:
    def __init__(self, player_number):
        self.player_number = player_number
        self.type = 'human'
        self.player_string = 'Player {}:human'.format(player_number)

    def get_move(self, board):
        """
        Given the current board state returns the human input for next move

        INPUTS:
        board - a numpy array containing the state of the board using the
                following encoding:
                - the board maintains its same two dimensions
                    - row 0 is the top of the board and so is
                      the last row filled
                - spaces that are unoccupied are marked as 0
                - spaces that are occupied by player 1 have a 1 in them
                - spaces that are occupied by player 2 have a 2 in them

        RETURNS:
        The 0 based index of the column that represents the next move
        """

        valid_cols = []
        for i, col in enumerate(board.T):
            if 0 in col:
                valid_cols.append(i)

        move = int(input('Enter your move: '))

        while move not in valid_cols:
            print('Column full, choose from:{}'.format(valid_cols))
            move = int(input('Enter your move: '))

        return move

