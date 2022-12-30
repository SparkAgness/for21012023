bool switch_helper(bool increm, bool decrem, bool switch_flag) {
//helps to avoid effect of changing clock's values during menu_switcher() using
    if ((!decrem || !increm) && switch_flag) {
        return 1;
    }
    else
        return 0;
}

/*m.b the function() have to return digital value, for example:
val 1 - implementation of increm
val 2 - implementation of decrem
val 3 - implementation of menu_switcher

appropriately, type returned value must be uint8_t*/

