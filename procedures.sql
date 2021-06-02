
CREATE OR REPLACE PROCEDURE find_customer (cus_id IN NUMBER, found OUT NUMBER) AS 

customer_name customers.name%type;

BEGIN 
    customer_name := '';
    found := 0;

    SELECT customers.name into customer_name from customers where customer_id = cus_id;
    if (customer_name != 'a') then 
        found := 1;
    else
        found := 0;
    end if;
    EXCEPTION

WHEN no_data_found THEN

found := 0;

END;


CREATE OR REPLACE PROCEDURE find_product (prod_id IN NUMBER, price OUT products.list_price%TYPE) AS

BEGIN 
    
    price := 0;

    SELECT products.list_price into price from products where product_id = prod_id;
    
    EXCEPTION

WHEN no_data_found THEN

price := 0;

END;

CREATE OR REPLACE PROCEDURE add_order (customer_id IN NUMBER, new_order_id OUT NUMBER) as

ord_id orders.order_id%type;

BEGIN

select max(order_id) into ord_id from orders;

ord_id := ord_id + 1;

new_order_id := ord_id;

INSERT INTO ORDERS (ORDER_ID,CUSTOMER_ID,STATUS,SALESMAN_ID,ORDER_DATE) VALUES (ord_id, customer_id,'Shipped',56,sysdate);

END;

CREATE OR REPLACE PROCEDURE add_order_item (orderId IN order_items.order_id%type, itemId IN order_items.item_id%type, productId IN order_items.product_id%type, quantity IN order_items.quantity%type, price IN order_items.unit_price%type) as

begin

INSERT INTO ORDER_ITEMS (ORDER_ID,ITEM_ID,PRODUCT_ID,QUANTITY,UNIT_PRICE) VALUES (orderId,itemId,productId,quantity,price);

END;