#include "chomik.h"
#include "config.h"
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <string>

//#define CHOMIK_DEBUG

#ifdef CHOMIK_DEBUG
#define DEBUG(X) std::cout << __FILE__ << " line " << __LINE__ << ":" << X << "\n"
#else
#define DEBUG(X)
#endif

#define CHOMIK_MACHINE_STDERR(X) *chomik::machine::current_error_stream << X


void chomik::machine::create_predefined_variables()
{
    DEBUG("machine::create_predefined_variables");

    generic_name gn;
    gn.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    gn.add_generic_name_item(std::make_shared<identifier_name_item>("target"));
    gn.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_print_target_stream_index=std::make_shared<signature>(gn);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_print_target_stream_index), 0));

    generic_name gn2;
    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn2.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_created_stream_index=std::make_shared<signature>(gn2);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_created_stream_index), 0));

    generic_name gn3;
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn3.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_get_from_stream_result_stream_index=std::make_shared<signature>(gn3);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_get_from_stream_result_stream_index), 0));

    generic_name gn4;
    gn4.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn4.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn4.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn4.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn4.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_from_stream_result=std::make_shared<signature>(gn4);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_get_from_stream_result), ""));


    generic_name gn5;
    gn5.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn5.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    gn5.add_generic_name_item(std::make_shared<identifier_name_item>("separator"));
    std::shared_ptr<signature> the_print_separator=std::make_shared<signature>(gn5);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_print_separator), " "));

    generic_name gn6;
    gn6.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn6.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    gn6.add_generic_name_item(std::make_shared<identifier_name_item>("end"));
    gn6.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    gn6.add_generic_name_item(std::make_shared<identifier_name_item>("line"));
    std::shared_ptr<signature> the_print_end_of_line=std::make_shared<signature>(gn6);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_print_end_of_line), "\n"));

    generic_name gn7;
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("source"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn7.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_read_from_stream_source_stream_index=std::make_shared<signature>(gn7);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_read_from_stream_source_stream_index), 2));  // standard input

    generic_name gn8;
    gn8.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn8.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    gn8.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn8.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn8.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn8.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_read_from_stream_result_integer=std::make_shared<signature>(gn8);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_read_from_stream_result_integer), 0));

    generic_name gn9;
    gn9.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn9.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    gn9.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn9.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn9.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn9.add_generic_name_item(std::make_shared<name_item_string>("string"));
    std::shared_ptr<signature> the_read_from_stream_result_string=std::make_shared<signature>(gn9);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_read_from_stream_result_string), ""));

    generic_name gn10;
    gn10.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn10.add_generic_name_item(std::make_shared<identifier_name_item>("compare"));
    gn10.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_compare_result=std::make_shared<signature>(gn10);
    add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_compare_result), ""));

    generic_name gn11;
    gn11.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn11.add_generic_name_item(std::make_shared<identifier_name_item>("add"));
    gn11.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn11.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_add_result_integer=std::make_shared<signature>(gn11);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_add_result_integer), 0));

    generic_name gn12;
    gn12.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn12.add_generic_name_item(std::make_shared<identifier_name_item>("subtract"));
    gn12.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn12.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_subtract_result_integer=std::make_shared<signature>(gn12);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_subtract_result_integer), 0));

    generic_name gn13;
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("set"));
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("to"));
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn13.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_set_to_stream_stream_index=std::make_shared<signature>(gn13);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_set_to_stream_stream_index), 0));

    generic_name gn14;
    gn14.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn14.add_generic_name_item(std::make_shared<identifier_name_item>("break"));
    gn14.add_generic_name_item(std::make_shared<identifier_name_item>("flag"));
    std::shared_ptr<signature> the_break_flag=std::make_shared<signature>(gn14);
    add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_break_flag), "false"));


    generic_name gn15;
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("max"));
    gn15.add_generic_name_item(std::make_shared<identifier_name_item>("size"));
    std::shared_ptr<signature> the_read_from_stream_max_size=std::make_shared<signature>(gn15);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_read_from_stream_max_size), 0));


    generic_name gn16;
    gn16.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn16.add_generic_name_item(std::make_shared<identifier_name_item>("program"));
    gn16.add_generic_name_item(std::make_shared<identifier_name_item>("return"));
    std::shared_ptr<signature> the_program_return=std::make_shared<signature>(gn16);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_program_return), 0));


    generic_name gn17;
    gn17.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn17.add_generic_name_item(std::make_shared<identifier_name_item>("multiply"));
    gn17.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn17.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_multiply_result_integer=std::make_shared<signature>(gn17);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_multiply_result_integer), 1));

    generic_name gn18;
    gn18.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn18.add_generic_name_item(std::make_shared<identifier_name_item>("divide"));
    gn18.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn18.add_generic_name_item(std::make_shared<name_item_string>("float"));
    std::shared_ptr<signature> the_divide_result_float=std::make_shared<signature>(gn18);
    add_variable_with_value(std::make_shared<simple_variable_with_value_float>(std::move(the_divide_result_float), 1));

    generic_name gn19;
    gn19.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn19.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn19.add_generic_name_item(std::make_shared<identifier_name_item>("is"));
    gn19.add_generic_name_item(std::make_shared<identifier_name_item>("defined"));
    gn19.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_is_defined_result=std::make_shared<signature>(gn19);
    add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_get_is_defined_result), "false"));

    generic_name gn20;
    gn20.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn20.add_generic_name_item(std::make_shared<identifier_name_item>("chomik"));
    gn20.add_generic_name_item(std::make_shared<identifier_name_item>("package"));
    gn20.add_generic_name_item(std::make_shared<identifier_name_item>("version"));
    std::shared_ptr<signature> the_chomik_package_version=std::make_shared<signature>(gn20);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_chomik_package_version), PACKAGE_VERSION));


    generic_name gn21;
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("ad"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("hoc"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("types"));
    gn21.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_amount_of_ad_hoc_types_result=std::make_shared<signature>(gn21);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_get_amount_of_ad_hoc_types_result), 0));

    generic_name gn22;
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("variables"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("in"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("memory"));
    gn22.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_amount_of_variables_in_the_memory_result=std::make_shared<signature>(gn22);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_get_amount_of_variables_in_the_memory_result), 0));

    generic_name gn23;
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("items"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("in"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("memory"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("variables"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    gn23.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_amount_of_items_in_the_memory_variables_signature_result=std::make_shared<signature>(gn23);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_get_amount_of_items_in_the_memory_variables_signature_result), 0));

    generic_name gn24;
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("item"));
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("representation"));
    gn24.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_signature_item_representation_result=std::make_shared<signature>(gn24);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_get_signature_item_representation_result), ""));

    generic_name gn25;
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("item"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("types"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("name"));
    gn25.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_get_signature_item_types_name_result=std::make_shared<signature>(gn25);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_get_signature_item_types_name_result), ""));


    generic_name gn26;
    gn26.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn26.add_generic_name_item(std::make_shared<identifier_name_item>("getline"));
    gn26.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn26.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_getline_stream_index=std::make_shared<signature>(gn26);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_getline_stream_index), 2));  // 2 is the standard input


    generic_name gn27;
    gn27.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn27.add_generic_name_item(std::make_shared<identifier_name_item>("getline"));
    gn27.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_getline_result=std::make_shared<signature>(gn27);
    add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_getline_result), ""));

    generic_name gn28;
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("regular"));
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("expression"));
    gn28.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_created_signature_regular_expression_index=std::make_shared<signature>(gn28);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_created_signature_regular_expression_index), 0));  //

    for (int i = 1; i<=max_match_group_index; i++)
    {
        generic_name gn29;
        gn29.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
        gn29.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
        gn29.add_generic_name_item(std::make_shared<identifier_name_item>("group"));
        gn29.add_generic_name_item(std::make_shared<name_item_string>("integer"));
        gn29.add_generic_name_item(std::make_shared<name_item_integer>(i));
        std::shared_ptr<signature> the_match_group_integer_x=std::make_shared<signature>(gn29);
        add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_match_group_integer_x), 0));

        generic_name gn292;
        gn292.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
        gn292.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
        gn292.add_generic_name_item(std::make_shared<identifier_name_item>("group"));
        gn292.add_generic_name_item(std::make_shared<name_item_string>("boolean"));
        gn292.add_generic_name_item(std::make_shared<name_item_integer>(i));
        std::shared_ptr<signature> the_match_group_boolean_x=std::make_shared<signature>(gn292);
        add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_match_group_boolean_x), "false"));
    }

    generic_name gn30;
    gn30.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn30.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
    gn30.add_generic_name_item(std::make_shared<identifier_name_item>("expression"));
    gn30.add_generic_name_item(std::make_shared<identifier_name_item>("index"));
    std::shared_ptr<signature> the_match_expression_index=std::make_shared<signature>(gn30);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_match_expression_index), 0));

    generic_name gn31;
    gn31.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn31.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
    gn31.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    std::shared_ptr<signature> the_match_result=std::make_shared<signature>(gn31);
    add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_match_result), "false"));


    generic_name gn32;
    gn32.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn32.add_generic_name_item(std::make_shared<identifier_name_item>("modulo"));
    gn32.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn32.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_modulo_result_integer=std::make_shared<signature>(gn32);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_modulo_result_integer), 0));

    generic_name gn33;
    gn33.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn33.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    gn33.add_generic_name_item(std::make_shared<identifier_name_item>("is"));
    gn33.add_generic_name_item(std::make_shared<identifier_name_item>("good"));
    std::shared_ptr<signature> the_stream_is_good=std::make_shared<signature>(gn33);
    add_variable_with_value(std::make_shared<simple_variable_with_value_enum>(std::move(the_stream_is_good), "true"));

    generic_name gn34;
    gn34.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn34.add_generic_name_item(std::make_shared<identifier_name_item>("cast"));
    gn34.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn34.add_generic_name_item(std::make_shared<name_item_string>("integer"));
    std::shared_ptr<signature> the_cast_result_integer=std::make_shared<signature>(gn34);
    add_variable_with_value(std::make_shared<simple_variable_with_value_integer>(std::move(the_cast_result_integer), 0));

    generic_name gn35;
    gn35.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn35.add_generic_name_item(std::make_shared<identifier_name_item>("multiply"));
    gn35.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn35.add_generic_name_item(std::make_shared<name_item_string>("float"));
    std::shared_ptr<signature> the_multiply_result_float=std::make_shared<signature>(gn35);
    add_variable_with_value(std::make_shared<simple_variable_with_value_float>(std::move(the_multiply_result_float), 0.0));

    generic_name gn36;
    gn36.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn36.add_generic_name_item(std::make_shared<identifier_name_item>("cast"));
    gn36.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn36.add_generic_name_item(std::make_shared<name_item_string>("float"));
    std::shared_ptr<signature> the_cast_result_float=std::make_shared<signature>(gn36);
    add_variable_with_value(std::make_shared<simple_variable_with_value_float>(std::move(the_cast_result_float), 0.0));

    generic_name gn37;
    gn37.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn37.add_generic_name_item(std::make_shared<identifier_name_item>("add"));
    gn37.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn37.add_generic_name_item(std::make_shared<name_item_string>("float"));
    std::shared_ptr<signature> the_add_result_float=std::make_shared<signature>(gn37);
    add_variable_with_value(std::make_shared<simple_variable_with_value_float>(std::move(the_add_result_float), 0.0));

    generic_name gn38;
    gn38.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    gn38.add_generic_name_item(std::make_shared<identifier_name_item>("subtract"));
    gn38.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    gn38.add_generic_name_item(std::make_shared<name_item_string>("float"));
    std::shared_ptr<signature> the_subtract_result_float=std::make_shared<signature>(gn38);
    add_variable_with_value(std::make_shared<simple_variable_with_value_float>(std::move(the_subtract_result_float), 0.0));

	if (vector_of_chomik_string_values.size() >= max_chomik_string_index)
	{
		for (int i = 1; i<=max_chomik_string_index; i++)
		{
			generic_name gn39;
			gn39.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
			gn39.add_generic_name_item(std::make_shared<identifier_name_item>("chomik"));
			gn39.add_generic_name_item(std::make_shared<name_item_string>("string"));
			gn39.add_generic_name_item(std::make_shared<name_item_integer>(i));

			std::shared_ptr<signature> the_chomik_string_x=std::make_shared<signature>(gn39);
			add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_chomik_string_x), vector_of_chomik_string_values[i-1]));
		}
	}

	generic_name gn40;
	gn40.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
	gn40.add_generic_name_item(std::make_shared<identifier_name_item>("concatenate"));
	gn40.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
	std::shared_ptr<signature> the_concatenate_result=std::make_shared<signature>(gn40);
	add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_concatenate_result), ""));

	generic_name gn41;
	gn41.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
	gn41.add_generic_name_item(std::make_shared<identifier_name_item>("concatenate"));
	gn41.add_generic_name_item(std::make_shared<identifier_name_item>("separator"));
	std::shared_ptr<signature> the_concatenate_separator=std::make_shared<signature>(gn41);
	add_variable_with_value(std::make_shared<simple_variable_with_value_string>(std::move(the_concatenate_separator), ""));
}

void chomik::machine::create_predefined_types()
{
    std::shared_ptr<type_instance> boolean_type = std::make_shared<type_instance_enum>("boolean");

    generic_name gn_false;
    gn_false.add_generic_name_item(std::make_shared<identifier_name_item>("false"));
    signature signature_false{gn_false};
    boolean_type->add_type_instance_enum_value(signature_false);

    generic_name gn_true;
    gn_true.add_generic_name_item(std::make_shared<identifier_name_item>("true"));
    signature signature_true{gn_true};
    boolean_type->add_type_instance_enum_value(signature_true);

    // also create auxilliary variables for the newly created types
    create_auxilliary_variables_for_type_instance(*boolean_type);

    add_type_instance(std::move(boolean_type));


    std::shared_ptr<type_instance> comparison_result_type = std::make_shared<type_instance_enum>("compare_result");
    generic_name gn_lower;
    gn_lower.add_generic_name_item(std::make_shared<identifier_name_item>("lower"));
    signature signature_lower{gn_lower};
    comparison_result_type->add_type_instance_enum_value(signature_lower);

    generic_name gn_equal;
    gn_equal.add_generic_name_item(std::make_shared<identifier_name_item>("equal"));
    signature signature_equal{gn_equal};
    comparison_result_type->add_type_instance_enum_value(signature_equal);

    generic_name gn_greater;
    gn_greater.add_generic_name_item(std::make_shared<identifier_name_item>("greater"));
    signature signature_greater{gn_greater};
    comparison_result_type->add_type_instance_enum_value(signature_greater);

    // also create auxilliary variables for the newly created types
    create_auxilliary_variables_for_type_instance(*comparison_result_type);

    add_type_instance(std::move(comparison_result_type));
}

void chomik::machine::create_predefined_streams()
{
    DEBUG("machine::create_predefined_streams");

    add_stream(std::make_unique<generic_stream_standard_output_stream>());
    add_stream(std::make_unique<generic_stream_standard_error_stream>());
    add_stream(std::make_unique<generic_stream_standard_input_stream>());

}


chomik::signature_common_data::signature_common_data()
{
    generic_name_the_print_target_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_print_target_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    generic_name_the_print_target_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("target"));
    generic_name_the_print_target_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_print_target_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_print_separator.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_print_separator.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    generic_name_the_print_separator.add_generic_name_item(std::make_shared<identifier_name_item>("separator"));

    generic_name_the_print_end_of_line.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_print_end_of_line.add_generic_name_item(std::make_shared<identifier_name_item>("print"));
    generic_name_the_print_end_of_line.add_generic_name_item(std::make_shared<identifier_name_item>("end"));
    generic_name_the_print_end_of_line.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    generic_name_the_print_end_of_line.add_generic_name_item(std::make_shared<identifier_name_item>("line"));

    generic_name_the_created_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_created_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
    generic_name_the_created_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_created_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("set"));
    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("to"));
    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_set_to_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_get_from_stream_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_get_from_stream_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_from_stream_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_from_stream_result.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_get_from_stream_result.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_get_from_stream_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));


    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("source"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_read_from_stream_source_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_read_from_stream_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));

    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_read_from_stream_result_string.add_generic_name_item(std::make_shared<name_item_string>("string"));


    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("max"));
    generic_name_the_read_from_stream_max_size.add_generic_name_item(std::make_shared<identifier_name_item>("size"));

    generic_name_the_compare_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_compare_result.add_generic_name_item(std::make_shared<identifier_name_item>("compare"));
    generic_name_the_compare_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_multiply_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_multiply_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("multiply"));
    generic_name_the_multiply_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_multiply_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));


    generic_name_the_divide_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_divide_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("divide"));
    generic_name_the_divide_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_divide_result_float.add_generic_name_item(std::make_shared<name_item_string>("float"));


    generic_name_the_add_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_add_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("add"));
    generic_name_the_add_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_add_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));


    generic_name_the_subtract_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_subtract_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("subtract"));
    generic_name_the_subtract_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_subtract_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));


    generic_name_the_get_is_defined_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_is_defined_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_is_defined_result.add_generic_name_item(std::make_shared<identifier_name_item>("is"));
    generic_name_the_get_is_defined_result.add_generic_name_item(std::make_shared<identifier_name_item>("defined"));
    generic_name_the_get_is_defined_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("ad"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("hoc"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("types"));
    generic_name_the_get_amount_of_ad_hoc_types_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("variables"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("in"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("memory"));
    generic_name_the_get_amount_of_variables_in_the_memory_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("amount"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("of"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("items"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("in"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("memory"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("variables"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("item"));
    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("representation"));
    generic_name_the_get_signature_item_representation_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("get"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("item"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("types"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("name"));
    generic_name_the_get_signature_item_types_name_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_getline_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_getline_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("getline"));
    generic_name_the_getline_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_getline_stream_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_getline_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_getline_result.add_generic_name_item(std::make_shared<identifier_name_item>("getline"));
    generic_name_the_getline_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("created"));
    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("signature"));
    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("regular"));
    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("expression"));
    generic_name_the_created_signature_regular_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_match_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_match_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
    generic_name_the_match_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("expression"));
    generic_name_the_match_expression_index.add_generic_name_item(std::make_shared<identifier_name_item>("index"));

    generic_name_the_match_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_match_result.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
    generic_name_the_match_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

    generic_name_the_modulo_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_modulo_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("modulo"));
    generic_name_the_modulo_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_modulo_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));

    generic_name_the_stream_is_good.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_stream_is_good.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
    generic_name_the_stream_is_good.add_generic_name_item(std::make_shared<identifier_name_item>("is"));
    generic_name_the_stream_is_good.add_generic_name_item(std::make_shared<identifier_name_item>("good"));

    generic_name_the_cast_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_cast_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("cast"));
    generic_name_the_cast_result_integer.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_cast_result_integer.add_generic_name_item(std::make_shared<name_item_string>("integer"));

    generic_name_the_multiply_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_multiply_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("multiply"));
    generic_name_the_multiply_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_multiply_result_float.add_generic_name_item(std::make_shared<name_item_string>("float"));

    generic_name_the_cast_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_cast_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("cast"));
    generic_name_the_cast_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_cast_result_float.add_generic_name_item(std::make_shared<name_item_string>("float"));

    generic_name_the_add_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_add_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("add"));
    generic_name_the_add_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_add_result_float.add_generic_name_item(std::make_shared<name_item_string>("float"));

    generic_name_the_subtract_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
    generic_name_the_subtract_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("subtract"));
    generic_name_the_subtract_result_float.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
    generic_name_the_subtract_result_float.add_generic_name_item(std::make_shared<name_item_string>("float"));


	generic_name_the_concatenate_result.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
	generic_name_the_concatenate_result.add_generic_name_item(std::make_shared<identifier_name_item>("concatenate"));
	generic_name_the_concatenate_result.add_generic_name_item(std::make_shared<identifier_name_item>("result"));

	generic_name_the_concatenate_separator.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
	generic_name_the_concatenate_separator.add_generic_name_item(std::make_shared<identifier_name_item>("concatenate"));
	generic_name_the_concatenate_separator.add_generic_name_item(std::make_shared<identifier_name_item>("separator"));

    signature_the_print_target_stream_index = std::make_unique<signature>(generic_name_the_print_target_stream_index);
    signature_the_print_separator = std::make_unique<signature>(generic_name_the_print_separator);
    signature_the_print_end_of_line = std::make_unique<signature>(generic_name_the_print_end_of_line);
    signature_the_created_stream_index = std::make_unique<signature>(generic_name_the_created_stream_index);
    signature_the_set_to_stream_stream_index = std::make_unique<signature>(generic_name_the_set_to_stream_stream_index);
    signature_the_get_from_stream_stream_index = std::make_unique<signature>(generic_name_the_get_from_stream_stream_index);
    signature_the_get_from_stream_result = std::make_unique<signature>(generic_name_the_get_from_stream_result);
    signature_the_read_from_stream_source_stream_index = std::make_unique<signature>(generic_name_the_read_from_stream_source_stream_index);
    signature_the_read_from_stream_result_integer = std::make_unique<signature>(generic_name_the_read_from_stream_result_integer);
    signature_the_read_from_stream_result_string = std::make_unique<signature>(generic_name_the_read_from_stream_result_string);
    signature_the_read_from_stream_max_size = std::make_unique<signature>(generic_name_the_read_from_stream_max_size);
    signature_the_compare_result = std::make_unique<signature>(generic_name_the_compare_result);
    signature_the_multiply_result_integer = std::make_unique<signature>(generic_name_the_multiply_result_integer);
    signature_the_divide_result_float = std::make_unique<signature>(generic_name_the_divide_result_float);
    signature_the_add_result_integer = std::make_unique<signature>(generic_name_the_add_result_integer);
    signature_the_subtract_result_integer = std::make_unique<signature>(generic_name_the_subtract_result_integer);
    signature_the_get_is_defined_result = std::make_unique<signature>(generic_name_the_get_is_defined_result);
    signature_the_get_amount_of_ad_hoc_types_result = std::make_unique<signature>(generic_name_the_get_amount_of_ad_hoc_types_result);
    signature_the_get_amount_of_variables_in_the_memory_result = std::make_unique<signature>(generic_name_the_get_amount_of_variables_in_the_memory_result);
    signature_the_get_amount_of_items_in_the_memory_variables_signature_result =
    std::make_unique<signature>(generic_name_the_get_amount_of_items_in_the_memory_variables_signature_result);
    signature_the_get_signature_item_representation_result = std::make_unique<signature>(generic_name_the_get_signature_item_representation_result);
    signature_the_get_signature_item_types_name_result = std::make_unique<signature>(generic_name_the_get_signature_item_types_name_result);
    signature_the_getline_stream_index = std::make_unique<signature>(generic_name_the_getline_stream_index);
    signature_the_getline_result = std::make_unique<signature>(generic_name_the_getline_result);
    signature_the_created_signature_regular_expression_index = std::make_unique<signature>(generic_name_the_created_signature_regular_expression_index);
    signature_the_match_expression_index = std::make_unique<signature>(generic_name_the_match_expression_index);
    signature_the_match_result = std::make_unique<signature>(generic_name_the_match_result);
    signature_the_modulo_result_integer = std::make_unique<signature>(generic_name_the_modulo_result_integer);
    signature_the_stream_is_good = std::make_unique<signature>(generic_name_the_stream_is_good);
    signature_the_cast_result_integer = std::make_unique<signature>(generic_name_the_cast_result_integer);
    signature_the_multiply_result_float = std::make_unique<signature>(generic_name_the_multiply_result_float);
    signature_the_cast_result_float = std::make_unique<signature>(generic_name_the_cast_result_float);
    signature_the_add_result_float = std::make_unique<signature>(generic_name_the_add_result_float);
    signature_the_subtract_result_float = std::make_unique<signature>(generic_name_the_subtract_result_float);

    for (int i=1; i<=machine::max_match_group_index; i++)
    {
        generic_name gn;
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
        gn.add_generic_name_item(std::make_shared<identifier_name_item>("group"));
        gn.add_generic_name_item(std::make_shared<name_item_string>("integer"));
        gn.add_generic_name_item(std::make_shared<name_item_integer>(i));
        std::unique_ptr<signature> the_match_group_integer_x=std::make_unique<signature>(gn);
        signature_the_match_group_integer_x.push_back(std::move(the_match_group_integer_x));

        generic_name gn2;
        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("match"));
        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("group"));
        gn2.add_generic_name_item(std::make_shared<name_item_string>("boolean"));
        gn2.add_generic_name_item(std::make_shared<name_item_integer>(i));
        std::unique_ptr<signature> the_match_group_boolean_x=std::make_unique<signature>(gn2);
        signature_the_match_group_boolean_x.push_back(std::move(the_match_group_boolean_x));
    }

    for (int i=1; i<=machine::max_chomik_string_index; i++)
	{
		generic_name gn;
		gn.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
		gn.add_generic_name_item(std::make_shared<identifier_name_item>("chomik"));
		gn.add_generic_name_item(std::make_shared<name_item_string>("string"));
		gn.add_generic_name_item(std::make_shared<name_item_integer>(i));

		std::unique_ptr<signature> the_chomik_string_x=std::make_unique<signature>(gn);
		signature_the_chomik_string_x.push_back(std::move(the_chomik_string_x));
	}

	signature_the_concatenate_result = std::make_unique<signature>(generic_name_the_concatenate_result);
	signature_the_concatenate_separator = std::make_unique<signature>(generic_name_the_concatenate_separator);

}

void chomik::signature::execute_predefined_print(machine & m) const
{
    int index = m.get_variable_with_value(*our_common_data->signature_the_print_target_stream_index).get_value_integer();

    DEBUG("value of the print target stream index " << index);

    if (index >= 0 && index < m.get_amount_of_streams())
    {
        std::string separator = m.get_variable_with_value(*our_common_data->signature_the_print_separator).get_value_string();

        DEBUG("the print separator is \"" << separator << "\"");

        std::string end_of_line = m.get_variable_with_value(*our_common_data->signature_the_print_end_of_line).get_value_string();

        generic_stream& gs{m.get_stream(index)};

        if (vector_of_items.size() == 3
            && vector_of_items[1]->get_it_is_identifier("memory")
            && vector_of_items[2]->get_it_is_identifier("report"))
        {
            m.report(gs.get_output_stream());
        }
        else
        {
            bool first = true;
            for (auto & i: vector_of_items)
            {
                if (first)
                {
                    first = false;
                    continue;
                }
                i->print(gs.get_output_stream());
                gs.get_output_stream() << separator;
            }
            gs.get_output_stream() << end_of_line;
        }
    }
}

void chomik::signature::execute_predefined_create(machine & m) const
{
    if (vector_of_items.size() == 5)
    {
        if (vector_of_items[1]->get_it_is_identifier("new")
            && vector_of_items[2]->get_it_is_identifier("output")
            && vector_of_items[3]->get_it_is_identifier("filestream")
            && vector_of_items[4]->get_it_is_string())
        {
            if (m.get_can_create_files())
            {
                m.add_stream(std::make_unique<generic_stream_file_output>(vector_of_items[4]->get_value_string()));
            }
            else
            {
                m.add_stream(std::make_unique<generic_stream_stringstream>());
            }
            generic_stream & gs = m.get_stream(m.get_last_created_stream_index());

            DEBUG("assign value integer " << m.get_last_created_stream_index());

            m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());

            set_stream_flags(m, gs);

            return;
        }
        else
            if (vector_of_items[1]->get_it_is_identifier("new")
                && vector_of_items[2]->get_it_is_identifier("input")
                && vector_of_items[3]->get_it_is_identifier("filestream")
                && vector_of_items[4]->get_it_is_string())
            {
                if (m.get_can_create_files())
                {
                    m.add_stream(std::make_unique<generic_stream_file_input>(vector_of_items[4]->get_value_string()));
                }
                else
                {
                    m.add_stream(std::make_unique<generic_stream_stringstream>());
                }

                generic_stream & gs = m.get_stream(m.get_last_created_stream_index());

                DEBUG("assign value integer " << m.get_last_created_stream_index());

                m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());
                set_stream_flags(m, gs);
                return;
            }
            else
                if (vector_of_items[1]->get_it_is_identifier("new")
                    && vector_of_items[2]->get_it_is_identifier("input")
                    && vector_of_items[3]->get_it_is_identifier("stringstream")
                    && vector_of_items[4]->get_it_is_string())
                {
                    m.add_stream(std::make_unique<generic_stream_stringstream>(vector_of_items[4]->get_value_string()));

                    generic_stream & gs = m.get_stream(m.get_last_created_stream_index());

                    DEBUG("assign value integer " << m.get_last_created_stream_index());

                    m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());
                    set_stream_flags(m, gs);
                    return;
                }
    }
    else
        if (vector_of_items.size() == 4)
        {
            if (vector_of_items[1]->get_it_is_identifier("new")
                && vector_of_items[2]->get_it_is_identifier("output")
                && vector_of_items[3]->get_it_is_identifier("stringstream"))
            {
                m.add_stream(std::make_unique<generic_stream_stringstream>());

                generic_stream & gs = m.get_stream(m.get_last_created_stream_index());

                DEBUG("assign value integer " << m.get_last_created_stream_index());

                m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());

                set_stream_flags(m, gs);

                return;
            }
        }
        else
            if (vector_of_items.size() == 6)
            {
                if (vector_of_items[1]->get_it_is_identifier("new")
                    && vector_of_items[2]->get_it_is_identifier("signature")
                    && vector_of_items[3]->get_it_is_identifier("regular")
                    && vector_of_items[4]->get_it_is_identifier("expression")
                    && vector_of_items[5]->get_it_is_string()
                )
                {
                    // here we parse the signature regular expression
                    m.add_signature_regular_expression(std::make_unique<signature_regular_expression>(vector_of_items[5]->get_value_string()));

                    DEBUG("assign value integer " << m.get_last_created_signature_regular_expression_index());

                    m.get_variable_with_value(
                        *our_common_data->signature_the_created_signature_regular_expression_index)
                    .assign_value_integer(m.get_last_created_signature_regular_expression_index());
                    return;
                }
            }
            else
                if (vector_of_items.size() == 9)
                {
                    if (vector_of_items[1]->get_it_is_identifier("new")
                        && vector_of_items[2]->get_it_is_identifier("input")
                        && vector_of_items[3]->get_it_is_identifier("random")
                        && vector_of_items[4]->get_it_is_identifier("number")
                        && vector_of_items[5]->get_it_is_identifier("stream")
                        && vector_of_items[6]->get_it_is_string()
                        && vector_of_items[7]->get_it_is_integer()
                        && vector_of_items[8]->get_it_is_integer()
                    )
                    {
                        if (vector_of_items[6]->get_value_string() == "integer")
                        {
                            m.add_stream(std::make_unique<generic_stream_random_number_stream>(vector_of_items[7]->get_value_integer(), vector_of_items[8]->get_value_integer()));

                            generic_stream & gs = m.get_stream(m.get_last_created_stream_index());

                            DEBUG("assign value integer " << m.get_last_created_stream_index());

                            m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());

                            set_stream_flags(m, gs);

                            return;
                        }
                    }
                }
                else
                    if (vector_of_items.size() == 7)
                    {
                        if (vector_of_items[1]->get_it_is_identifier("new")
                            && vector_of_items[2]->get_it_is_identifier("input")
                            && vector_of_items[3]->get_it_is_identifier("random")
                            && vector_of_items[4]->get_it_is_identifier("enum")
                            && vector_of_items[5]->get_it_is_identifier("stream")
                            && vector_of_items[6]->get_it_is_string())  // enum type name
                        {
                            m.add_stream(std::make_unique<generic_stream_random_enum_stream>(vector_of_items[6]->get_value_string(), m));

                            generic_stream & gs = m.get_stream(m.get_last_created_stream_index());

                            DEBUG("assign value integer " << m.get_last_created_stream_index());

                            m.get_variable_with_value(*our_common_data->signature_the_created_stream_index).assign_value_integer(m.get_last_created_stream_index());

                            set_stream_flags(m, gs);
                            return;
                        }
                    }

                    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown create variable\n");
                    for (auto & i: vector_of_items)
                    {
                        CHOMIK_MACHINE_STDERR(*i << ' ');
                    }
                    CHOMIK_MACHINE_STDERR('\n');
}


void chomik::signature::execute_predefined_set(machine & m) const
{
    if (vector_of_items.size() == 4)
    {
        if (vector_of_items[1]->get_it_is_identifier("to")
            && vector_of_items[2]->get_it_is_identifier("stream")
            && vector_of_items[3]->get_it_is_string())
        {

            //the set to stream result stream index

            int index = m.get_variable_with_value(*our_common_data->signature_the_set_to_stream_stream_index).get_value_integer();

            DEBUG("value of the set to stream stream index " << index);
            if (index >= 0 && index < m.get_amount_of_streams())
            {
                generic_stream& gs{m.get_stream(index)};

                gs.set_result(vector_of_items[3]->get_value_string());

                DEBUG("assign value string " << gs.get_result());

                set_stream_flags(m, gs);

                return;
            }
            return;
        }
    }

    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown set variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');
}


void chomik::signature::execute_predefined_getline(machine & m) const
{
    if (vector_of_items.size() == 1)
    {
        //the get from stream result stream index
        int index = m.get_variable_with_value(*our_common_data->signature_the_getline_stream_index).get_value_integer();

        DEBUG("value of the getline stream index " << index);
        if (index >= 0 && index < m.get_amount_of_streams())
        {
            generic_stream& gs{m.get_stream(index)};

            gs.getline();

            std::string result = gs.getline_result();

            DEBUG("assign value string " << result);

            m.get_variable_with_value(*our_common_data->signature_the_getline_result).assign_value_string(result);

            set_stream_flags(m, gs);

            return;
        }
        return;
    }

    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown getline variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');

}

void chomik::signature::execute_predefined_get(machine & m) const
{
    if (vector_of_items.size() == 3)
    {
        if (vector_of_items[1]->get_it_is_identifier("from")
            && vector_of_items[2]->get_it_is_identifier("stream"))
        {

            //the get from stream result stream index
            int index = m.get_variable_with_value(*our_common_data->signature_the_get_from_stream_stream_index).get_value_integer();

            DEBUG("value of the get from stream stream index " << index);
            if (index >= 0 && index < m.get_amount_of_streams())
            {
                generic_stream& gs{m.get_stream(index)};

                DEBUG("assign value string " << gs.get_result());

                m.get_variable_with_value(*our_common_data->signature_the_get_from_stream_result).assign_value_string(gs.get_result());

                set_stream_flags(m, gs);
                return;
            }
            return;
        }
    }


    if (vector_of_items.size() == 6)
    {
        if (vector_of_items[1]->get_it_is_identifier("amount")
            && vector_of_items[2]->get_it_is_identifier("of")
            && vector_of_items[3]->get_it_is_identifier("ad")
            && vector_of_items[4]->get_it_is_identifier("hoc")
            && vector_of_items[5]->get_it_is_identifier("types")
        )
        {
            m.get_variable_with_value(*our_common_data->signature_the_get_amount_of_ad_hoc_types_result).assign_value_integer(m.get_amount_of_ad_hoc_type_instances());
            return;
        }

        // get signature item representation (VARIABLE_INDEX:integer) (ITEM_INDEX:integer)
        if (vector_of_items[1]->get_it_is_identifier("signature")
            && vector_of_items[2]->get_it_is_identifier("item")
            && vector_of_items[3]->get_it_is_identifier("representation")
            && vector_of_items[4]->get_it_is_integer()  // index of the memory variable
            && vector_of_items[5]->get_it_is_integer()  // index of the item
        )
        {
            m.get_variable_with_value(*our_common_data->
            signature_the_get_signature_item_representation_result).assign_value_string(
                m.get_signature_item_representation(vector_of_items[4]->get_value_integer(), vector_of_items[5]->get_value_integer()));
            return;
        }

    }

    if (vector_of_items.size() == 7)
    {
        if (vector_of_items[1]->get_it_is_identifier("amount")
            && vector_of_items[2]->get_it_is_identifier("of")
            && vector_of_items[3]->get_it_is_identifier("variables")
            && vector_of_items[4]->get_it_is_identifier("in")
            && vector_of_items[5]->get_it_is_identifier("the")
            && vector_of_items[6]->get_it_is_identifier("memory")
        )
        {
            m.get_variable_with_value(*our_common_data->signature_the_get_amount_of_variables_in_the_memory_result)
            .assign_value_integer(m.get_amount_of_variables_in_the_memory());
            return;
        }

        if (vector_of_items[1]->get_it_is_identifier("signature")
            && vector_of_items[2]->get_it_is_identifier("item")
            && vector_of_items[3]->get_it_is_identifier("types")
            && vector_of_items[4]->get_it_is_identifier("name")
            && vector_of_items[5]->get_it_is_integer() // index of the memory variable
            && vector_of_items[6]->get_it_is_integer() // index of the item
        )
        {
            m.get_variable_with_value(*our_common_data->signature_the_get_signature_item_types_name_result)
            .assign_value_string(m.get_signature_item_type_name(vector_of_items[5]->get_value_integer(), vector_of_items[6]->get_value_integer()));
            return;
        }
    }


    if (vector_of_items.size() == 10)
    {
        if (vector_of_items[1]->get_it_is_identifier("amount")
            && vector_of_items[2]->get_it_is_identifier("of")
            && vector_of_items[3]->get_it_is_identifier("items")
            && vector_of_items[4]->get_it_is_identifier("in")
            && vector_of_items[5]->get_it_is_identifier("the")
            && vector_of_items[6]->get_it_is_identifier("memory")
            && vector_of_items[7]->get_it_is_identifier("variables")
            && vector_of_items[8]->get_it_is_identifier("signature")
            && vector_of_items[9]->get_it_is_integer())
        {
            m.get_variable_with_value(*our_common_data->signature_the_get_amount_of_items_in_the_memory_variables_signature_result)
            .assign_value_integer(m.get_amount_of_items_in_the_memory_variables_signature(vector_of_items[9]->get_value_integer()));
            return;
        }
    }

    /* here we implement the family of variables <get is defined ...> which sets the boolean value <the get is defined result>. */
    if (vector_of_items.size() >= 4)
    {
        if (vector_of_items[1]->get_it_is_identifier("is")
            && vector_of_items[2]->get_it_is_identifier("defined"))
        {

            signature sn;
            for (auto i = 3; i<vector_of_items.size(); i++)
            {
                std::shared_ptr<signature_item> c;
                vector_of_items[i]->get_copy(c);
                sn.add_content(std::move(c));
            }
            if (m.get_variable_is_represented_in_memory(sn) && m.get_variable_with_value(*our_common_data->signature_the_get_is_defined_result).get_value_enum() == "true")
            {
                //m.get_variable_with_value(*our_common_data->signature_the_get_is_defined_result).assign_value_enum("true");
            }
            else
            {
                m.get_variable_with_value(*our_common_data->signature_the_get_is_defined_result).assign_value_enum("false");

                /*
                 *                        std::cout << "the variable is represented in memory: " << m.get_variable_is_represented_in_memory(sn) << " and the result is "
                 *                            << m.get_variable_with_value(*our_common_data->signature_the_get_is_defined_result).get_value_enum() << "\n";
                 */
            }
            return;
        }
    }

    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown get variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');
}

void chomik::signature::execute_predefined_read(machine & m) const
{
    if (vector_of_items.size() == 4)
    {
        if (vector_of_items[1]->get_it_is_identifier("from")
            && vector_of_items[2]->get_it_is_identifier("stream")
            && vector_of_items[3]->get_it_is_string())
        {

            if (vector_of_items[3]->get_value_string() == "integer")
            {
                int index = m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_source_stream_index).get_value_integer();

                DEBUG("value of the read from stream source stream index " << index);
                if (index >= 0 && index < m.get_amount_of_streams())
                {
                    generic_stream& gs{m.get_stream(index)};

                    int s = gs.read_integer();

                    DEBUG("read " << s);

                    m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_result_integer).assign_value_integer(s);

                    set_stream_flags(m, gs);
                    return;
                }
            }
            else
                if (vector_of_items[3]->get_value_string() == "string")
                {
                    int index = m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_source_stream_index).get_value_integer();

                    DEBUG("value of the print target stream index " << index);
                    if (index >= 0 && index < m.get_amount_of_streams())
                    {
                        generic_stream& gs{m.get_stream(index)};

                        int max_size = m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_max_size).get_value_integer();

                        gs.set_max_size(max_size);

                        std::string s = gs.read_string();

                        DEBUG("read " << s);

                        m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_result_string).assign_value_string(s);

                        set_stream_flags(m, gs);
                        return;
                    }
                }
                else
                {
                    int index = m.get_variable_with_value(*our_common_data->signature_the_read_from_stream_source_stream_index).get_value_integer();

                    DEBUG("value of the print target stream index " << index);
                    if (index >= 0 && index < m.get_amount_of_streams())
                    {
                        generic_stream& gs{m.get_stream(index)};

                        generic_name gn2;
                        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("the"));
                        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("read"));
                        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("from"));
                        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("stream"));
                        gn2.add_generic_name_item(std::make_shared<identifier_name_item>("result"));
                        gn2.add_generic_name_item(std::make_shared<name_item_string>(vector_of_items[3]->get_value_string()));
                        signature the_read_from_stream_result{gn2};

                        std::string s = gs.read_string();

                        DEBUG("read " << s);

                        m.get_variable_with_value(the_read_from_stream_result).assign_value_enum(s);
                        set_stream_flags(m, gs);
                    }
                }
                return;
        }
    }
    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown read variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');
}

void chomik::signature::execute_predefined_compare(machine & m) const
{
    if (vector_of_items.size() == 4)
    {

        if (vector_of_items[1]->get_it_is_string()
            && vector_of_items[2]->get_it_is_integer()
            && vector_of_items[3]->get_it_is_integer())
        {
            if (vector_of_items[1]->get_value_string() == "integer")
            {
                std::string s = "";

                int a = vector_of_items[2]->get_value_integer();
                int b = vector_of_items[3]->get_value_integer();

                DEBUG("got " << a << " and " << b);

                if (a < b)
                {
                    s = "lower";
                }
                else
                    if (a > b)
                    {
                        s = "greater";
                    }
                    else
                    {
                        s = "equal";
                    }

                    m.get_variable_with_value(*our_common_data->signature_the_compare_result).assign_value_enum(s);
                return;
            }
        }
        else
            if (vector_of_items[1]->get_it_is_string()
                && vector_of_items[2]->get_it_is_float()
                && vector_of_items[3]->get_it_is_float())
            {
                if (vector_of_items[1]->get_value_string() == "float")
                {
                    std::string s = "";

                    double a = vector_of_items[2]->get_value_float();
                    double b = vector_of_items[3]->get_value_float();

                    DEBUG("got " << std::showpoint << a << " and " << std::showpoint << b);

                    if (a < b)
                    {
                        s = "lower";
                    }
                    else
                        if (a > b)
                        {
                            s = "greater";
                        }
                        else
                        {
                            s = "equal";    // comparison of floats (in fact doubles) for equality makes little sense, but we allow it
                        }

                        m.get_variable_with_value(*our_common_data->signature_the_compare_result).assign_value_enum(s);
                    return;
                }
            }
            else
                if (vector_of_items[1]->get_it_is_string()
                    && vector_of_items[2]->get_it_is_enum()
                    && vector_of_items[3]->get_it_is_enum())
                {
                    std::string s = "";
                    std::string a = vector_of_items[2]->get_value_enum();
                    std::string b = vector_of_items[3]->get_value_enum();

                    auto x{strcmp(a.c_str(), b.c_str())};

                    if (x < 0)
                    {
                        s = "lower";
                    }
                    else
                        if (x > 0)
                        {
                            s = "greater";
                        }
                        else
                        {
                            s = "equal";
                        }

                        m.get_variable_with_value(*our_common_data->signature_the_compare_result).assign_value_enum(s);
                    return;
                }
                else
                    if (vector_of_items[1]->get_it_is_string()
                        && vector_of_items[2]->get_it_is_string()
                        && vector_of_items[3]->get_it_is_string())
                    {
                        std::string s = "";
                        std::string a = vector_of_items[2]->get_value_string();
                        std::string b = vector_of_items[3]->get_value_string();

                        auto x{strcmp(a.c_str(), b.c_str())};

                        if (x < 0)
                        {
                            s = "lower";
                        }
                        else
                            if (x > 0)
                            {
                                s = "greater";
                            }
                            else
                            {
                                s = "equal";
                            }

                            m.get_variable_with_value(*our_common_data->signature_the_compare_result).assign_value_enum(s);
                        return;
                    }
    }
    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown compare variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');
}


void chomik::signature::execute_predefined_multiply(machine & m) const
{
    if (vector_of_items.size() == 4)
    {
        if (vector_of_items[1]->get_it_is_string()
            && vector_of_items[2]->get_it_is_integer()
            && vector_of_items[3]->get_it_is_integer())
        {
            if (vector_of_items[1]->get_value_string() == "integer")
            {
                std::string s = "";

                int a = vector_of_items[2]->get_value_integer();
                int b = vector_of_items[3]->get_value_integer();

                DEBUG("signature::execute_predefined_multiply got " << a << " and " << b);

                m.get_variable_with_value(*our_common_data->signature_the_multiply_result_integer).assign_value_integer(a*b);
                return;
            }
        }
        else
            if (vector_of_items[1]->get_it_is_string()
                && vector_of_items[2]->get_it_is_float()
                && vector_of_items[3]->get_it_is_float())
            {
                if (vector_of_items[1]->get_value_string() == "float")
                {
                    std::string s = "";

                    double a = vector_of_items[2]->get_value_float();
                    double b = vector_of_items[3]->get_value_float();

                    DEBUG("signature::execute_predefined_multiply got " << a << " and " << b);

                    m.get_variable_with_value(*our_common_data->signature_the_multiply_result_float).assign_value_float(a*b);
                    return;
                }
            }
    }
    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown multiply variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');
}


void chomik::signature::execute_predefined_modulo(machine & m) const
{
    if (vector_of_items.size() == 4)
    {
        if (vector_of_items[1]->get_it_is_string()
            && vector_of_items[2]->get_it_is_integer()
            && vector_of_items[3]->get_it_is_integer())
        {
            if (vector_of_items[1]->get_value_string() == "integer")
            {
                std::string s = "";

                int a = vector_of_items[2]->get_value_integer();
                int b = vector_of_items[3]->get_value_integer();

                DEBUG("signature::execute_predefined_modulo got " << a << " and " << b);

                if (b!=0)
                {
                    m.get_variable_with_value(*our_common_data->signature_the_modulo_result_integer).assign_value_integer(a%b);
                }
                return;
            }
        }
    }
    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown modulo variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');

}



void chomik::signature::execute_predefined_divide(machine & m) const
{
    if (vector_of_items.size() == 4)
    {
        if (vector_of_items[1]->get_it_is_string()
            && vector_of_items[2]->get_it_is_integer()
            && vector_of_items[3]->get_it_is_integer())
        {
            if (vector_of_items[1]->get_value_string() == "float")
            {
                std::string s = "";

                int a = vector_of_items[2]->get_value_integer();
                int b = vector_of_items[3]->get_value_integer();

                DEBUG("signature::execute_predefined_divide got " << a << " and " << b);

                if (b!=0)
                {
                    m.get_variable_with_value(*our_common_data->signature_the_divide_result_float).assign_value_float(static_cast<double>(a)/static_cast<double>(b));
                }

                return;
            }
        }
        else
            if (vector_of_items[1]->get_it_is_string()
                && vector_of_items[2]->get_it_is_float()
                && vector_of_items[3]->get_it_is_float())
            {
                if (vector_of_items[1]->get_value_string() == "float")
                {
                    std::string s = "";

                    double a = vector_of_items[2]->get_value_float();
                    double b = vector_of_items[3]->get_value_float();

                    DEBUG("signature::execute_predefined_divide got " << a << " and " << b);

                    if (b<-0.0001 || b>0.0001) // we should use limits
                    {
                        m.get_variable_with_value(*our_common_data->signature_the_divide_result_float).assign_value_float(a/b);
                    }
                    return;
                }
            }
    }
    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown divide variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');

}


void chomik::signature::execute_predefined_add(machine & m) const
{
    if (vector_of_items.size() == 4)
    {
        if (vector_of_items[1]->get_it_is_string()
            && vector_of_items[2]->get_it_is_integer()
            && vector_of_items[3]->get_it_is_integer())
        {
            if (vector_of_items[1]->get_value_string() == "integer")
            {
                std::string s = "";

                int a = vector_of_items[2]->get_value_integer();
                int b = vector_of_items[3]->get_value_integer();

                DEBUG("signature::execute_predefined_add got " << a << " and " << b);

                m.get_variable_with_value(*our_common_data->signature_the_add_result_integer).assign_value_integer(a+b);
                return;
            }
        }
        else
            if (vector_of_items[1]->get_it_is_string()
                && vector_of_items[2]->get_it_is_float()
                && vector_of_items[3]->get_it_is_float())
            {
                if (vector_of_items[1]->get_value_string() == "float")
                {
                    std::string s = "";

                    double a = vector_of_items[2]->get_value_float();
                    double b = vector_of_items[3]->get_value_float();

                    DEBUG("signature::execute_predefined_add got " << a << " and " << b);

                    m.get_variable_with_value(*our_common_data->signature_the_add_result_float).assign_value_float(a+b);
                    return;
                }
            }
    }
    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown add variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');
}

void chomik::signature::execute_predefined_subtract(machine & m) const
{
    if (vector_of_items.size() == 4)
    {
        if (vector_of_items[1]->get_it_is_string()
            && vector_of_items[2]->get_it_is_integer()
            && vector_of_items[3]->get_it_is_integer())
        {
            if (vector_of_items[1]->get_value_string() == "integer")
            {
                std::string s = "";

                int a = vector_of_items[2]->get_value_integer();
                int b = vector_of_items[3]->get_value_integer();

                DEBUG("got " << a << " and " << b);

                m.get_variable_with_value(*our_common_data->signature_the_subtract_result_integer).assign_value_integer(a-b);
                return;
            }
        }
        else
            if (vector_of_items[1]->get_it_is_string()
                && vector_of_items[2]->get_it_is_float()
                && vector_of_items[3]->get_it_is_float())
            {
                if (vector_of_items[1]->get_value_string() == "float")
                {
                    std::string s = "";

                    double a = vector_of_items[2]->get_value_float();
                    double b = vector_of_items[3]->get_value_float();

                    DEBUG("got " << a << " and " << b);

                    m.get_variable_with_value(*our_common_data->signature_the_subtract_result_float).assign_value_float(a-b);
                    return;
                }
            }
    }
    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown subtract variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');
}

void chomik::signature::execute_predefined_execution(machine & m) const
{
    if (vector_of_items.size() == 6)
    {
        if (vector_of_items[1]->get_it_is_identifier("of")
            && vector_of_items[2]->get_it_is_identifier("the")
            && vector_of_items[3]->get_it_is_identifier("memory")
            && vector_of_items[4]->get_it_is_identifier("variables")
            && vector_of_items[5]->get_it_is_integer())
        {
            int index = vector_of_items[5]->get_value_integer();


            if (index >= 0 && index <m.get_amount_of_variables_in_the_memory())
            {
                DEBUG("execution of the memory variables " << index);

                code c;
                m.get_variable_with_value_by_index(index).get_value_code(c);
                c.execute(m);
                return;
            }
        }
    }

    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown execution variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');
}


void chomik::signature::execute_predefined_match(machine & m) const
{
    if (vector_of_items.size() >= 2)
    {
        int index = m.get_variable_with_value(*our_common_data->signature_the_match_expression_index).get_value_integer();

        DEBUG("value of the match expression index " << index);
        if (index >= 0 && index < m.get_amount_of_signature_regular_expressions())
        {
            std::stringstream matched_string_stream;
            for (int i=1; i<vector_of_items.size(); i++)
            {
                matched_string_stream << vector_of_items[i]->get_string_representation();
            }
            std::string matched_string{matched_string_stream.str()};

            signature_regular_expression& se{m.get_signature_regular_expression(index)};
            std::smatch my_smatch;

            bool result = std::regex_match(matched_string, my_smatch, se.get_regular_expression());

            if (result)
            {
                for (int i=1; i<my_smatch.size(); i++)
                {
                    if (i < machine::max_match_group_index)
                    {
                        //std::cout << "got " << my_smatch[i] << "\n";

                        try {

                            if (my_smatch[i] == "false" || my_smatch[i] == "true")
                            {
                                m.get_variable_with_value(*our_common_data->signature_the_match_group_boolean_x[i - 1]).assign_value_enum(my_smatch[i]);
                            }
                            else
                            {
                                m.get_variable_with_value(*our_common_data->signature_the_match_group_integer_x[i - 1]).assign_value_integer(std::stoi(my_smatch[i]));
                            }
                        }
                        catch (...)
                        {
                            CHOMIK_MACHINE_STDERR("error on " << my_smatch[i] << '\n');
                        }
                    }
                }
            }
            m.get_variable_with_value(*our_common_data->signature_the_match_result).assign_value_enum(result ? "true" : "false");
        }
        return;
    }

    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown match variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');
}


void chomik::signature::execute_predefined_cast(machine & m) const
{
    if (vector_of_items.size() == 5)
    {
        if (vector_of_items[1]->get_it_is_string()
            && vector_of_items[2]->get_it_is_identifier("to")
            && vector_of_items[3]->get_it_is_string()
            && vector_of_items[4]->get_it_is_float())
        {
            if (vector_of_items[1]->get_value_string() == "float"
                && vector_of_items[3]->get_value_string() == "integer")
            {
                double operand = vector_of_items[4]->get_value_float();

                DEBUG("cast " << operand << " to " << static_cast<int>(operand));

                m.get_variable_with_value(*our_common_data->signature_the_cast_result_integer).assign_value_integer(static_cast<int>(operand));

                return;
            }
        }
        else
            if (vector_of_items[1]->get_it_is_string()
                && vector_of_items[2]->get_it_is_identifier("to")
                && vector_of_items[3]->get_it_is_string()
                && vector_of_items[4]->get_it_is_integer())
            {
                if (vector_of_items[1]->get_value_string() == "integer"
                    && vector_of_items[3]->get_value_string() == "float")
                {
                    int operand = vector_of_items[4]->get_value_integer();

                    DEBUG("cast " << operand << " to " << static_cast<double>(operand));

                    m.get_variable_with_value(*our_common_data->signature_the_cast_result_float).assign_value_float(static_cast<double>(operand));
                    return;
                }
            }

    }

    CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown cast variable\n");
    for (auto & i: vector_of_items)
    {
        CHOMIK_MACHINE_STDERR(*i << ' ');
    }
    CHOMIK_MACHINE_STDERR('\n');
}


void chomik::signature::execute_predefined_change(machine & m) const
{
	if (vector_of_items.size() == 3)
	{
		if (vector_of_items[1]->get_it_is_identifier("directory")
			&& vector_of_items[2]->get_it_is_string())
		{
			std::string target_directory = vector_of_items[2]->get_value_string();

			DEBUG("change directory to " << target_directory);

			//std::cout << "change directory to " << target_directory << "??\n";

			if (m.get_can_change_directory())
			{
					DEBUG("and we can do it! " << target_directory);

					//std::cout << "change directory to " << target_directory << "\n";

					chdir(target_directory.c_str());
			}
			else
			{
				DEBUG("no, we cannot do it!");
				//std::cout << "we cannot change directory to " << target_directory << "\n";
			}
			return;
		}
	}

	CHOMIK_MACHINE_STDERR("line " << line_number << " : unknown change variable\n");
	for (auto & i: vector_of_items)
	{
		CHOMIK_MACHINE_STDERR(*i << ' ');
	}
	CHOMIK_MACHINE_STDERR('\n');
}


void chomik::signature::execute_predefined_concatenate(machine & m) const
{
		std::stringstream s;

		std::string separator = m.get_variable_with_value(*our_common_data->signature_the_concatenate_separator).get_value_string();

		DEBUG("the concatenate separator is \"" << separator << "\"");

		bool first = true, second = true;
		for (auto & i: vector_of_items)
		{
			if (first)
			{
				first = false;
				continue; // we omit 'concatenate'
			}
			else
			{
				if (second)
				{
					second = false;
				}
				else
				{
					s << separator;
				}
			}
			i->print(s);
		}

		DEBUG("concatenate produced " << s.str());

		m.get_variable_with_value(*our_common_data->signature_the_concatenate_result).assign_value_string(s.str());
}


void chomik::signature::execute_predefined(machine & m) const
{
    if (m.get_is_user_defined_executable(*this))
    {
        m.execute_user_defined_executable(*this);
    }
    else
    if (get_it_has_prefix("print"))
    {
        execute_predefined_print(m);
    }
    else
    if (get_it_has_prefix("create"))
    {
        execute_predefined_create(m);
    }
    else
    if (get_it_has_prefix("get"))
    {
        execute_predefined_get(m);
    }
    else
    if (get_it_has_prefix("read"))
    {
        execute_predefined_read(m);
    }
    else
    if (get_it_has_prefix("compare"))
    {
        execute_predefined_compare(m);
    }
    else
    if (get_it_has_prefix("add"))
    {
        execute_predefined_add(m);
    }
    else
    if (get_it_has_prefix("modulo"))
    {
        execute_predefined_modulo(m);
    }
    else
    if (get_it_has_prefix("subtract"))
    {
        execute_predefined_subtract(m);
    }
    else
    if (get_it_has_prefix("multiply"))
    {
        execute_predefined_multiply(m);
    }
    else
    if (get_it_has_prefix("divide"))
    {
        execute_predefined_divide(m);
    }
    else
    if (get_it_has_prefix("set"))
    {
        execute_predefined_set(m);
    }
    else
    if (get_it_has_prefix("getline"))
    {
        execute_predefined_getline(m);
    }
    else
    if (get_it_has_prefix("execution"))
    {
        execute_predefined_execution(m);
    }
    else
    if (get_it_has_prefix("match"))
    {
        execute_predefined_match(m);
    }
    else
    if (get_it_has_prefix("cast"))
    {
        execute_predefined_cast(m);
    }
    else
	if (get_it_has_prefix("change"))
	{
		execute_predefined_change(m);
	}
	else
	if (get_it_has_prefix("concatenate"))
	{
		execute_predefined_concatenate(m);
	}
}

const std::string chomik::predefined_variables::array_of_predefined_variables[]=
{
	"print", "create", "get", "read", "compare", "add", "subtract", "multiply", "divide", "set", "getline", "execution", "match", "modulo", "cast", "change",
	"concatenate"
};

bool chomik::predefined_variables::get_variable_is_predefined(const std::string & prefix)
{
	for (auto & i: predefined_variables::array_of_predefined_variables)
	{
		if (i == prefix)
		{
			return true;
		}
	}
	return false;
}

