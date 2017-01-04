struct lexemeTableEntry * converter(struct token * lexemes){
    int i = 0;
    struct lexemeTableEntry * here = NULL;
    struct lexemeTableEntry * there = here;
    for(i = 0; i < token_num; i++){
        here = (struct lexemeTableEntry * )malloc(sizeof(struct lexemeTableEntry));
        here -> lexeme = lexemes[i].string;
        here -> lexemeLen = lexemes[i].size;
        here -> token = lexemes[i].id;
        if(lexemes[i].id == 2 || lexemes[i].id == 3){
            here = here -> next;
            here = (struct lexemeTableEntry * )malloc(sizeof(struct lexemeTableEntry));
            itoa(variable_id, here -> lexeme, 10);
            here -> lexemeLen = strlen(itoa(variable_id, here -> lexeme, 10));
            here -> lexemes[i].id;
        }
        here = here -> next;
        
    }
    return there;
}
