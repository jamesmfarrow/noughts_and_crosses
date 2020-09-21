#ifndef TICTACTOE_HPP
#define TICTACTOE_HPP


#include <cstdlib>
#include <ctime>
#include <climits>
#include <iostream>

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>


class controller;


class game_move
{
private:
    int m_x{};
    int m_y{};

public:
    //score public for easy access
    int score;

    //default constructor
    game_move() { }

    //constructor
    game_move(int x,int y) : m_x(x), m_y(y){ }

    void setX(int x) { m_x = x; }
    void setY(int y) { m_y = y; }
    int getX() { return m_x; }
    int getY() { return m_y; }
    void setScore(int val) { score = val; }
    int getScore();

    //equality operator overload
    bool operator==(const game_move& obj)
    {
        return (this->m_x == obj.m_x && this->m_y == obj.m_y);
    }


};

// underlying game code logic, doesn't know anything about view
class model {

private:

    controller* c;
    char board[3][3]={{' ', ' ', ' '},{' ', ' ', ' '},{' ', ' ', ' '}};
    int turn;
    static int level;

public:
    bool PlayerXwin = false;
    bool PlayerOwin = false;

    //constructor
    model(controller* c) : c(c), turn(0) { srand(time(0)); }

    void reset() {
        for( int i = 0; i < 9;i++)
        {
            board[i/3][i%3] = ' ';
        }
        turn = 0;
        PlayerXwin = false;
        PlayerOwin = false;
    }

    void setlevel(int input) { level = input; }

    //Human move
    void move(int row, int col);

    //random AI move
    void random_move();

    //MINMAX AI move
    void AI_move();

    bool is_a_draw()
    {
        return (turn > 8 && (!PlayerXwin || !PlayerOwin));
    }

    bool who_won()
    {
        //iterate through all 8 winning combinations to check for a win
        for(int i{};i < 3;i++)
        {
            //horizontal first
            if(board[i][0] == board[i][1] && board[i][1] == board[i][2])
            {
                if(board[i][0] == 'X') { PlayerXwin = true; return true; }
                else if(board[i][0] == 'O') { PlayerOwin = true; return true; }
            }
            //now vertical
            else if(board[0][i] == board[1][i] && board[1][i] == board[2][i])
            {
                if(board[0][i] == 'X') { PlayerXwin = true; return true; }
                else if(board[0][i] == 'O') { PlayerOwin = true; return true; }
            }
        }

        //now diagonals
        if(board[0][0] == board[1][1] && board[1][1] == board[2][2])
        {
            if(board[0][0] == 'X') { PlayerXwin = true; return true; }
            else if(board[0][0] == 'O') { PlayerOwin = true; return true; }
        }
        else if(board[0][2] == board[1][1] && board[1][1] == board[2][0])
        {
            if(board[0][2] == 'X') { PlayerXwin = true; return true; }
            else if(board[0][2] == 'O') { PlayerOwin = true; return true; }
        }

        return false;

    }//end function


    //this nested class is for AI logic in MINMAX algorithm
    //everything below is for the AI player

    bool win()
    {
        //iterate through all 8 winning combinations to check for a win
        for(int i{};i < 3;i++)
        {
            //horizontal first
            if(board[i][0] == board[i][1] && board[i][1] == board[i][2])
            {
                if(board[i][0] != ' ') return true;
            }
            //now vertical
            else if(board[0][i] == board[1][i] && board[1][i] == board[2][i])
            {
                if(board[0][i] != ' ') return true;
            }
        }

        //now diagonals
        if(board[0][0] == board[1][1] && board[1][1] == board[2][2])
        {
            if(board[0][0] != ' ') return true;
        }
        else if(board[0][2] == board[1][1] && board[1][1] == board[2][0])
        {
            if(board[0][2] != ' ') return true;
        }

        return false;

    }

    bool draw()
    {

        //if this is true then it can't b a draw
        if (win()) return false;

        //iterate through the board to find an unoccupied square
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (board[i][j] == ' ')return false;
            }
        }

        //get here then it is a draw
        return true;
    }

    int eval()
    {

        int val{}, ev{};
        int playerX{};
        int playerO{};

        for(int i{}; i < 3;i++)
        {
            playerX=0;
            playerO=0;
            for(int j{};j < 3;j++)//3 in a row X/) +/- infinity
            {
                if(board[i][j] == 'X') ++playerX;
                if(board[i][j] == 'O') ++playerO;
            }
            ev = evalRowCol(playerX, playerO);
            if(ev > 2 || ev < -2) return ev;
            val += ev;

            playerX=0;
            playerO=0;

            for(int j{};j < 3;j++)//3 in a col X/) +/- infinity
            {
                if(board[j][i] == 'X') ++playerX;
                if(board[j][i] == 'O') ++playerO;
            }

            ev = evalRowCol(playerX, playerO);
            if(ev > 2 || ev < -2) return ev;
            val += ev;
        }

        playerX=0;
        playerO=0;

        for(int i=0; i< 3; i++)//major diagonal
        {
            if(board[i][i] == 'X') ++playerX;
            if(board[i][i] == 'O') ++playerO;
        }

        ev = evalRowCol(playerX, playerO);
        if(ev > 2 || ev < -2) return ev;
        val += ev;


        playerX=0;
        playerO=0;
        for(int i=0; i< 3; i++)//minor diagonal
        {
            if(board[i][2-i] == 'X') ++playerX;
            if(board[i][2-i] == 'O') ++playerO;
        }

        ev = evalRowCol(playerX, playerO);
        if(ev>2 || ev<-2) return ev;
        val += ev;

        return val;
    }

    int evalRowCol(int playerX, int playerO){
        if(playerX == 3) return -INT_MAX;
        else if(playerO == 3) return INT_MAX;
        else if(playerX == 2 && playerO == 0) return -2;
        else if (playerO == 2 && playerX == 0) return 2;
        else if(playerX == 1 && playerO == 0) return -1;
        else if(playerO == 1 && playerX == 0) return 1;


        return 0;
    }

    game_move minimax(bool maximising_player = true, int depth = level, int alpha = -INT_MAX, int beta = INT_MAX)
    {
        game_move best_move;

        //check for winning situation
        bool isWin = win();
        //check for a draw
        bool is_draw = draw();

        if(isWin || is_draw || depth==0)
        {
            if(isWin) { best_move.score = maximising_player ? -INT_MAX : INT_MAX; }
            else if(is_draw) { best_move.score = 0; }
            else { best_move.score = eval(); }

            return best_move;
        }

        if(maximising_player) best_move.score = -INT_MAX;
        else best_move.score = INT_MAX;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if(board[i][j] == ' ')//check to see if space on board and dont forget to reset
                {
                    //yes then set that space on the board to the correct maximising player
                    if(maximising_player) board[i][j] = 'O';
                    else  board[i][j] = 'X';
                    //make a move using minimising player
                    game_move board_state = minimax(!maximising_player, depth - 1, alpha, beta);
                    //which move gives best score?
                    //set best_move to the best score
                    if(maximising_player)
                    {
                        if (board_state.score > best_move.score)
                        {
                            best_move.score = board_state.score;
                            best_move.setX(i);
                            best_move.setY(j);
                            if(best_move.score > alpha) alpha = best_move.score;
                        }
                    }
                    else if(board_state.score < best_move.score)
                    {
                        best_move.score = board_state.score;
                        best_move.setX(i);
                        best_move.setY(j);
                        if(best_move.score < beta) beta = best_move.score;
                    }
                    //reset the board to original state
                    board[i][j] = ' ';
                }
            }
        }

        return best_move;
    }

};

int model::level = 1;

// gui code, doesn't know anything about game logic
class view : public QWidget {

private:

    QGridLayout layout;
    QPushButton* buttons[9];

    controller* c;

public:

    //constructor
    view(controller* c): layout(this), c(c) {
        init();
        setLayout(&layout);
        setup();
    }

    void init();

    void setup();

    void update(int row, int col, QString val) {
        buttons[row*3+col]->setText(val);
    }

    //funtion shows who won, play again, or quit
    void show_end_dialog(char w);

};



// controller intermediary, passes messages between view and model
class controller {

private:

    model* m;
    view* v;

public:

    controller(): m(new model(this)), v(new view(this)) {
        v->show();
    }

    void move(int row, int col) {
        m->move(row, col);
    }

    void update(int row, int col, char val) {
        QString str="";
        str+=val;
        v->update(row, col, str);
    }

    void end_game(char w) {
        v->show_end_dialog(w);
    }

    void reset() {
        m->reset();
    }

    void PlayingLevel(int num) {
        m->setlevel(num);
    }

};

// setup view, button presses call move method of controller
inline void view::setup() {
    for(int i=0; i<9; i++) {
        int row=i/3, col=i%3;
        buttons[i]=new QPushButton(this);
        buttons[i]->setFixedSize(QSize(200, 200));
        layout.addWidget(buttons[i], row, col);
        QObject::connect(buttons[i], &QPushButton::clicked, [=] {
            c->move(row, col);
        });
    }
}

// valid moves cause the model to tell the controller to update the view
inline void model::move(int row, int col) {
    //std::cout << row << " " << col << " " << turn << std::endl;
    if(board[row][col]==' ') {
        board[row][col]=(turn%2==0) ? 'X' : 'O';
        turn++;
        c->update(row, col, board[row][col]);
        //check game state, if draw or who won
        //pass X or O for winner, 0 for draw to controller function end_game()
        //controller function end_game() in turn calls view::show_end_dialog()
        //view::show_end_dialog() has option to quit game totally or click play again
        //that calls controller::reset() which calls model::reset() and resets game state
        if(who_won() || draw()) c->end_game(PlayerXwin ? 'X' : PlayerOwin ? 'O' : 0);
        if(turn%2==1) AI_move();
        }
    }


// random ai
inline void model::random_move() {
    int row, col;
    do {
        row=rand()%3, col=rand()%3;
    } while(board[row][col]!=' ');
    move(row, col);
}

//AI MINMAX move
inline void model::AI_move(){
   game_move AImove;
   AImove = minimax();
   move(AImove.getX(), AImove.getY());
}


inline void view::show_end_dialog(char w) {
    QString s="";
    if(w) {
        s+=w;
        s+=" won the game!";
    } else {
        s="it was a draw!";
    }
    QMessageBox b;
    QPushButton* quit = new QPushButton("Quit");
    QPushButton* play_again = new QPushButton("Play Again");
    b.setInformativeText(s);
    b.setText("Game Over");
    quit->setFixedSize(100,30);
    b.addButton(quit, QMessageBox::AcceptRole);
    b.addButton(play_again, QMessageBox::AcceptRole);
    QObject::connect(play_again, &QPushButton::pressed, [=](){
        c->reset();
        for(int i=0; i<9; i++)
            buttons[i]->setText("");
    });
    QObject::connect(quit, &QPushButton::pressed, [=](){
       QApplication::quit();
    });

    b.exec();

}

inline void view::init() {
    QMessageBox init;
    init.setInformativeText("Please click the level you want to play");
    init.setText("Welcome to Noughts and Crosses");
    QPushButton* easy = new QPushButton("Easy");
    QPushButton* hard = new QPushButton("Hard");
    init.addButton(easy, QMessageBox::AcceptRole);
    init.addButton(hard, QMessageBox::AcceptRole);
    easy->setFixedSize(150,30);
    hard->setFixedSize(150,30);
    QObject::connect(hard, &QPushButton::pressed, [=](){
        c->PlayingLevel(9);
    });
    QObject::connect(easy, &QPushButton::pressed, [=](){
       c->PlayingLevel(1);
    });

    init.exec();
    std::cout << "response" << std::endl;
}


#endif // TICTACTOE_HPP

