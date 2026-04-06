import React from 'react';
import { createSplitview, PROPERTY_KEYS_SPLITVIEW, } from 'dockview-core';
import { usePortalsLifecycle } from '../react';
import { ReactPanelView } from './view';
function extractCoreOptions(props) {
    const coreOptions = PROPERTY_KEYS_SPLITVIEW.reduce((obj, key) => {
        if (key in props) {
            obj[key] = props[key];
        }
        return obj;
    }, {});
    return coreOptions;
}
export const SplitviewReact = React.forwardRef((props, ref) => {
    const domRef = React.useRef(null);
    const splitviewRef = React.useRef();
    const [portals, addPortal] = usePortalsLifecycle();
    React.useImperativeHandle(ref, () => domRef.current, []);
    const prevProps = React.useRef({});
    React.useEffect(() => {
        const changes = {};
        PROPERTY_KEYS_SPLITVIEW.forEach((propKey) => {
            const key = propKey;
            const propValue = props[key];
            if (key in props && propValue !== prevProps.current[key]) {
                changes[key] = propValue;
            }
        });
        if (splitviewRef.current) {
            splitviewRef.current.updateOptions(changes);
        }
        else {
            // not yet fully initialized
        }
        prevProps.current = props;
    }, PROPERTY_KEYS_SPLITVIEW.map((key) => props[key]));
    React.useEffect(() => {
        if (!domRef.current) {
            return () => {
                // noop
            };
        }
        const frameworkOptions = {
            createComponent: (options) => {
                return new ReactPanelView(options.id, options.name, props.components[options.name], { addPortal });
            },
        };
        const api = createSplitview(domRef.current, Object.assign(Object.assign({}, extractCoreOptions(props)), frameworkOptions));
        const { clientWidth, clientHeight } = domRef.current;
        api.layout(clientWidth, clientHeight);
        if (props.onReady) {
            props.onReady({ api });
        }
        splitviewRef.current = api;
        return () => {
            splitviewRef.current = undefined;
            api.dispose();
        };
    }, []);
    React.useEffect(() => {
        if (!splitviewRef.current) {
            return;
        }
        splitviewRef.current.updateOptions({
            createComponent: (options) => {
                return new ReactPanelView(options.id, options.name, props.components[options.name], { addPortal });
            },
        });
    }, [props.components]);
    return (React.createElement("div", { style: { height: '100%', width: '100%' }, ref: domRef }, portals));
});
SplitviewReact.displayName = 'SplitviewComponent';
