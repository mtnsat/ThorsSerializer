
This directons some generic base classes for printing and parser.
They provide the framwork which you specialize for your specific file format.

    ScannerDom
        Generic scanner that generates a DOM
        Pass the ScannerDom a template that tells it how to make unique keys from an object.
        Pass the Parse method a template that tells it how to parse your file format.

        Usage:

            ScannerDom<JsonKey>             scanner;
            ParserObjectType    type = scanner.parser<JsonParser>(stream);
            switch(type)
            {
                case ParserMapObject:       scanner.getMap();       break;
                case ParserArrayObject:     scanner.getArray();     break;
                case ParserValueObject:     scanner.getValue();     break;
                case NotSet:                                        break;
            }

        Notes:
            KeyGenerator:   This should implement the KeyGenVisitor interface.
                            It is used to convert a ParserItem into a string when an element
                            is used as the key on a map element.

        It is normal for specific file formats to overirde the basic version with their own
        implementation details.

            See:
                JsonScannerDom          Uses a lex/yacc parser.
                JsonScannerDomAlt       Uses a hand built recursive desent parser.
                YamlScannerDom          Uses libyaml

    ScannerSax
        Generic scanner. The DOM scanner is implemente in terms of the SAX parser.
        So this is a lower level interface if you want to implement a lighter waight scanner.

        Usage:

            ScannerSax                      scanner;

            // Register the action you want to the scannner to take a different points.
            std::unique_ptr<SaxAction>      itemAction(new ItemAction);
            scanner.registerAction("item", itemAction);

            // Parse the source and take the actions.
            scanner.parser(stream);

        It is normal for specific file formats to overirde the basic version with their own
        implementation details.

            See:
                JsonScannerSax          Uses a lex/yacc parser.
                JsonScannerSaxAlt       Uses a hand built recursive desent parser.
                YamlScannerSax          Uses libyaml
        

    Dom:
        The DOM is a very simple generic DOM.
        There are three types of objects in the DOM
            Map
            Array
            Value
                enum ParserObjectType { ParserMapObject, ParserArrayObject, ParserValueObject, NotSet };

        The Value represents Strings/Number/Bool/Null/Map/Array
        You can traverse a DOM using ParserValueVisitor
            ParserValueVisitorYourVersion visitor;
            value.accept(visitor);

        From the value you can extract data with
            value.getValue<TypeYouWant>();
        If it can not covnert the node to your type an exception will be thrown.

    ParserInterface
        An interface called by the format speciifc parsers to indicate what part of the processes they
        have reached. The SAX/DOM parsers implement their own version of this interface. It is unlikely
        you will need to delve this deep and implement a ParserInterface

        But the format specific paser should be written to call this interface so that they can be used
        by different types of scanners.



