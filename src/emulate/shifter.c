#include "shifter.h"

shift_out setDefaultShift(uint32_t Rm_value) {
    shift_out result;
    result.data = Rm_value;
    result.carry = 0;
    return result;

}

shift_out lsl(int shift_value, uint32_t Rm_value) {
    shift_out result;
    if(0 == shift_value) {
        result = setDefaultShift(Rm_value);
    } else {

        if(shift_value > 1) {
            int num_of_shift = shift_value - 1;
            Rm_value = Rm_value << num_of_shift;
        }
        result.carry = (int)((Rm_value & check_bit31) >> 31);
        result.data = Rm_value << 1;
    }
    return result;
}

shift_out lsr(int shift_value, uint32_t Rm_value) {
    shift_out result;
    if(0 == shift_value) {
        result = setDefaultShift(Rm_value);
    } else {
        if(shift_value > 1) {
            int num_of_shift = shift_value - 1;
            Rm_value = Rm_value >> num_of_shift;
        }
        result.carry = (int)(Rm_value & check_bit0);
        result.data = Rm_value >> 1;
    }
    return result;
}

shift_out asr(int shift_value, uint32_t Rm_value) {
    shift_out result;
    uint32_t bit31 = check_bit31 & Rm_value;
    uint32_t bit31_extended = bit31;
    int counter = 0;
    while(counter < shift_value) {
        bit31 = bit31 >> 1 ;
        bit31_extended = bit31_extended | bit31;
        counter++;
    }

    result = lsr(shift_value, Rm_value);
    result.data = result.data | bit31_extended;
    return result;
}

shift_out ror(int shift_value, uint32_t Rm_value) {
    shift_out result;
    uint32_t temp;
    result.data = Rm_value;
    while(shift_value > 0) {
        temp = check_bit0 & result.data;
        temp = temp << 31;
        result.data = result.data >> 1;
        result.data = temp | result.data;
        shift_value--;
    }
    result.carry = (int)((result.data & check_bit31) >> 31);
    return result;
}

shift_out shifter(uint8_t shift, int Rm, state *st) {
    uint32_t Rm_value = st->reg[Rm];
    int types = (int)((check_shift_type & shift) >> 1);
    int shift_value;
    shift_out result;
    if(0 == (shift & check_bit4)) {
        /*read the shift value from bit 7 to 11*/
        shift_value = (int)((shift & check_shift_value) >> 3);
    }
    else {
        /*read the value from the Rs register*/
        int Rs = (int)((shift & check_Rs) >> 4);
        shift_value = st->reg[Rs];
    }

    switch(types) {
        case 0 :
            result = lsl(shift_value, Rm_value);
            break;
        case 1 :
            result = lsr(shift_value, Rm_value);
            break;
        case 2 :
            result = asr(shift_value, Rm_value);
            break;
        case 3 :
            result = ror(shift_value, Rm_value);
            break;
    }
    return result;
}

uint32_t immValue(int rotate_value, uint32_t immV) {
    uint32_t result;
    int num_rotate = 2 * rotate_value;
    result = ror(num_rotate, immV).data;
    return result;
}






